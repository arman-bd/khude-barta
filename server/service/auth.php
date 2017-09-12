<?php
require_once "mysql.php";
header("Access-Control-Allow-Origin: *");

if(isset($_REQUEST['auth']) and isset($_REQUEST['user']) and isset($_REQUEST['pass'])){
	// Authentication
	$user = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['user']);
	$pass = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['pass']);
			
	$SQL = "SELECT * FROM `user` WHERE `username` = '".
		   $user."' AND `password` = '".$pass."';";
	$MQ = mysqli_query($GLOBALS['DB'], $SQL);
	
	if(mysqli_num_rows($MQ) == 1){
		$MFA = mysqli_fetch_array($MQ);
		$user_id = $MFA['id'];
		$NewToken = md5(time().$_REQUEST['user']);
		$SQL = "UPDATE `user` SET `token` = '".$NewToken."' WHERE `id` = '".$user_id."'";
		$MQ2 = mysqli_query($GLOBALS['DB'], $SQL);
		
		// Last Messages
		$SQL = "SELECT `id` FROM `message` ORDER BY `id` DESC LIMIT 10, 1";
		$MQ3 = mysqli_query($GLOBALS['DB'], $SQL);
		if(mysqli_num_rows($MQ3) == 1){
			$MFA3 = mysqli_fetch_array($MQ3);
			$LastId = $MFA3['id'];
		}else{
			$LastId = 0;
		}
		
		// Login Successful
		echo "CONTINUE<:>".$NewToken."<:>".$MFA['id']."<:>".$MFA['username']."<:>".$MFA['name']."<:>".$LastId;
	}else{
		// Login Failed
		echo "FAILED<:>Error: Login Failed!";
	}
			
}



?>