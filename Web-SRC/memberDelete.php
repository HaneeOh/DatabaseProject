<?php
	$conn = mysqli_connect("127.0.0.1", "root", "1007", "myDB");
	/*
	if($db)
		echo "db 연결성공";
	else
		echo "db 연결 실패";
	*/
	if($_POST["id"] != "" && $_POST["psw"] != "")
	{
		$id = $_POST['id'];
		$psw =$_POST['psw'];
		
		$sql = "select * from member where m_id='$id' and m_psw='$psw'";
		$result = mysqli_query($conn, $sql);
		$exist = mysqli_num_rows($result);
		
		// 회원이 존재하는지 확인
		if($exist == 1)
		{
			$sql = "delete from member where m_id='$id' and m_psw='$psw'";
			
			if($conn->query($sql))
				echo "회원 탈퇴에 성공하였습니다.";
			else
				echo "회원 탈퇴에 실패하였습니다.";
		}
		else
		{
			echo "존재하지 않는 회원입니다.";
		}
	}
	
?>