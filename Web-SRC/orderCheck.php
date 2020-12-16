<?php
	session_start();
	
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	
?>

<html>
	<h3><?=$_SESSION['id']?>회원님의 주문내역</h3>
	
	<table width="400" border="1">
			<tr>
				<th>주문번호</th>
				<th>주문인</th>
				<th>상품 이름</th>
				<th>가격</th>
				<th>수량</th>
			</tr>

<?php
	$id = $_SESSION['id'];
	
	$sql = "select distinct o_id, m_name, p_name, p_price, o_p_num 
			from orderlist, member, product, cart
			where orderlist.o_m_id=member.m_id
			and orderlist.o_p_id=product.p_id
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
			</tr>";
	}
?>	
	</table>
</html>