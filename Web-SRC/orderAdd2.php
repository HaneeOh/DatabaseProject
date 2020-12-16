<?php
	session_start();
	
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	
	$id = $_SESSION['id'];
	
	$sql = "select * from orderlist";
	$result = mysqli_query($conn, $sql);
		
	$order_id = mysqli_num_rows($result);
	$order_id=$order_id+10;
	
	$sql = "select personalcart.p_name, c_p_num, p_stock, p_id 
			from personalcart, product
			where personalcart.p_name=product.p_name";
	
	$result = mysqli_query($conn, $sql);
	
	while ($row = mysqli_fetch_array($result))
	{
		if($row[2]>=$row[1])
		{
			$sql = "insert into orderlist values('$order_id', '$id', '$row[3]', '$row[1]')";
			
			if($conn->query($sql))
			{			
				$stock=$row[2]-$row[1];
				
				$sql = "update product set p_stock ='$stock' 
						where p_name = '$row[0]'";
				$result1 = mysqli_query($conn, $sql);
				
				$sql = "delete from cart 
						where c_id='$id'
						and c_p_id='$row[3]'";
				$result2 = mysqli_query($conn, $sql);
				
				$order_id++;
			}
			else
				echo "주문에 실패했습니다.";
		}
		else
			echo "'$row[0]' 상품은 재고 부족으로 주문하실 수 없습니다.";
	}
	
	echo "<h3>'$id'회원님의 전체 주문 내역.</h3>"
?>

<html>
<body>
	<br/>
	<table width="400" border="1">
		<tr>
			<th>주문번호</th>
			<th>주문인</th>
			<th>상품 이름</th>
			<th>가격</th>
			<th>수량</th>
		</tr>

<?php
	$sql = "select o_id, m_name, p_name, p_price, o_p_num 
			from orderlist, member, product
			where orderlist.o_m_id=member.m_id
			and orderlist.o_p_id = product.p_id
			and o_m_id='$id'";
			
	$result = mysqli_query($conn, $sql);
		
	while ($row = mysqli_fetch_array($result))
	{
		echo "<tr>
			<td align=center>$row[0]</td>
			<td align=center>$row[1]</td>
			<td align=center>$row[2]</td>
			<td align=center>$row[3]</td>
			<td align=center>$row[4]</td></td>
			</tr>";
	}
?>

	</table>
</html>