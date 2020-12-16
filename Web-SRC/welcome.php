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
	<h3><?=$_SESSION['id']?>회원님, 반갑습니다!</h3>
	<table width="400" border="1">
		<caption>상품 목록</caption>
			<tr>
				<th>분류</th>
				<th>상품 이름</th>
				<th>재고</th>
				<th>가격</th>
			</tr>

<?php
	$sql = "select * from product";
	$result = mysqli_query($conn, $sql);
	
	while ($row = mysqli_fetch_array($result))
	{
		echo "<tr>
			<td align=center>$row[2]</td>
			<td align=center>$row[1]</td>
			<td align=center>$row[3]</td>
			<td align=center>$row[4]</td>
			</tr>";
	}
?>		
	</table>
	<br/>
	수행할 작업의 버튼을 눌러주세요.<br/>
	<br/>
	<button type="button" onclick="location.href='cartAdd.html'"> 장바구니 담기 </button>
	<button type="button" onclick="location.href='cartCheck.html'"> 장바구니 확인 </button>
	<button type="button" onclick="location.href='orderAdd1.html'"> 주문하기 </button>
	<button type="button" onclick="location.href='orderCheck.php'"> 주문 확인 </button>
	<br/>
	<pre>	<button type="button" onclick="location.href='memberUpdate.html'"> 비밀번호 변경 </button>	<button type="button" onclick="location.href='logout.php'"> 로그아웃 </button></pre>
</body>
</html>

<?php
	$id = $_SESSION['id'];
	$sql = "create view personalcart as 
			select p_category, p_name, p_price, c_p_num 
			from cart, product
			where c_p_id=product.p_id
			and c_id='$id'";
			
	$result = mysqli_query($conn, $sql);
?>