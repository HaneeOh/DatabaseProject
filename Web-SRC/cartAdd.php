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
		
	$sql = "insert into cart values ('$id', '$p_id', '$num')";
	
	if($conn->query($sql))
		echo "<h3>'$id'회원님의 장바구니에 추가되었습니다.</h3>";
	else
		echo "<h3>장바구니에 추가하지 못했습니다.</h3>";
?>


<html>
<body>
	<br/>
	<table width="400" border="1">
		<tr>
			<th>분류</th>
			<th>상품 이름</th>
			<th>가격</th>
			<th>수량</th>
		</tr>

<?php
	$id = $_SESSION['id'];
	
	$sql = "select p_category, p_name, p_price, c_p_num 
			from cart, member, product
			where c_id=member.m_id
			and c_p_id=product.p_id
			and c_id='$id'";
			
	$result = mysqli_query($conn, $sql);
	
	while ($row = mysqli_fetch_array($result))
	{
		echo "<tr>
			<td align=center>$row[0]</td>
			<td align=center>$row[1]</td>
			<td align=center>$row[2]</td>
			<td align=center>$row[3]</td></td>
			</tr>";
	}
?>	
</html>