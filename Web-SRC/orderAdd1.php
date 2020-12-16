<?php
	session_start();
	
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	
	$p_id = $_POST['p_id'];
	$num = $_POST['p_num'];
	$id = $_SESSION['id'];
		
	$sql = "select p_stock from product
			where p_id='$p_id'";
	
	$result = mysqli_query($conn, $sql);
	
	while ($row = mysqli_fetch_array($result))
	{
		$stock =$row[0];
	}
	
	if($stock >= $num)
	{
		$sql = "select * from orderlist";
		$result = mysqli_query($conn, $sql);
	
		$order_id = mysqli_num_rows($result);
		$order_id=$order_id+10;
		
		$sql = "insert into orderlist values('$order_id', '$id', '$p_id', '$num')";
		
		if($conn->query($sql))
		{
			$stock -= $num;		
		
			$sql = "update product set p_stock ='$stock' 
					where p_id='$p_id'";
			
			if($conn->query($sql))
				echo "<h3>주문에 성공했습니다.</h3>";
		}
		else
			echo "<h3>주문에 실패했습니다.</h3>";
	}
	else
		echo "<h3>재고 부족으로 주문에 실패했습니다.</h3>";
?>


<html>
<body>
	<br/>
	<table width="400" border="1">
		<tr>
			<th>회원ID</th>
			<th>주문번호</th>
			<th>주문인</th>
			<th>상품 이름</th>
			<th>가격</th>
			<th>수량</th>
		</tr>

<?php
	$sql = "select distinct c_id, o_id, m_name, p_name, p_price, o_p_num 
			from orderlist, member, product, cart
			where orderlist.o_m_id=member.m_id
			and orderlist.o_p_id = product.p_id
			and cart.c_id=member.m_id
			and o_m_id='$id'";
			
	$result = mysqli_query($conn, $sql);
		
	while ($row = mysqli_fetch_array($result))
	{
		echo "<tr>
			<td align=center>$row[0]</td>
			<td align=center>$row[1]</td>
			<td align=center>$row[2]</td>
			<td align=center>$row[3]</td>
			<td align=center>$row[4]</td>
			<td align=center>$row[5]</td>
			</tr>";
	}
?>

	</table>
</html>