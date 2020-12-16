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

	if($_POST["psw1"] != "" && $_POST["psw2"] != "" )
	{
		$psw1 =$_POST['psw1'];
		$psw2 = $_POST['psw2'];
		
		if($psw1 == $psw2)
		{
			$psw = $psw1;
			$sql = "update member set m_psw ='$psw' 
					where m_id = '$id'";
		
			if($conn->query($sql))
				echo "회원 정보 수정에 성공하였습니다.";
			else
				echo "회원 정보 수정에 실패하였습니다.";
		}
	}
?>
