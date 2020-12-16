<html>
	<h3>회원 현황</h3>
	
	<table width="400" border="1">
		<tr>
			<th>ID</th>
			<th>이름</th>
			<th>전화번호</th>
		</tr>

<?php
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	
	$sql = "select m_id, m_name, m_phone from member";
	$result = mysqli_query($conn, $sql);
	
	while ($row = mysqli_fetch_array($result))
	{
		echo "<tr>
		<td align=center>$row[0]</td>
		<td align=center>$row[1]</td>
		<td align=center>$row[2]</td>
		</tr>";
	}
?>

</table>	
<br/>
</html>