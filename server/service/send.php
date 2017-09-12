<?php
require_once "mysql.php";
header("Access-Control-Allow-Origin: *");

if(isset($_REQUEST['token']) and isset($_REQUEST['message'])){
	// Authentication
	$token = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['token']);
	$message = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['message']);
			
	$SQL = "SELECT * FROM `user` WHERE `token` = '".$token."';";
	$MQ = mysqli_query($GLOBALS['DB'], $SQL);
	
	if(mysqli_num_rows($MQ) == 1){
		$MFA = mysqli_fetch_array($MQ);
		$user_id = $MFA['id'];
		
		$src = $user_id; // Sender
		$dst = 0; // Global
		$time = date("U");
		
		$SQL = "INSERT INTO `message` VALUES(NULL, $src, $dst, '".$message."', $time);";
		$MQ = mysqli_query($GLOBALS['DB'], $SQL);
		echo "SUCCESS";
	}else{
		// Login Failed
		echo "FAILED<:>Error: Authentication Failed";
	}
			
}else{
	echo "FAILED<:>Error: Invalid Request";
	print_r($_REQUEST);
	print_r($_GET);
}



?>