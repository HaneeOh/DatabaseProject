<?php
	session_start();
	
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	
	$sql="drop view personalcart";
	$result = mysqli_query($conn, $sql);

	session_destroy();
	header('Location: ./index.html');
?>
