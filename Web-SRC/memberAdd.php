<?php
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	
	if($_POST["id"] != "" && $_POST["psw1"] != "" && $_POST["psw2"] != "" && $_POST["name"] != "")
	{
		$id = $_POST['id'];
		$psw1 =$_POST['psw1'];
		$psw2 = $_POST['psw2'];
		$name = $_POST['name'];
		$phone = $_POST['phone'];
		
		$sql = "select * from member where m_id='$id'";
		$result = mysqli_query($conn, $sql);
		$exist = mysqli_num_rows($result);
		
		// ID 중복 검사
		if($exist == 1)
			echo "이미 존재하는 ID입니다.";
		else
		{
			if($psw1 == $psw2)
			{
				$psw = $psw1;
				$sql = "insert into member values ('$id', '$psw', '$name', '$phone')";
				
				if($conn->query($sql))
					echo "회원가입에 성공하였습니다.";
				else
					echo "회원가입에 실패하였습니다.";
			}
		}
	}
		
	
?>