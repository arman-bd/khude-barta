<?php
require_once "mysql.php";
header("Access-Control-Allow-Origin: *");

if(isset($_REQUEST['token']) and isset($_REQUEST['last'])){
	// Authentication
	$token = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['token']);
	$last = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['last']);
	
	$Message = array();
	
	$SQL = "SELECT * FROM `user` WHERE `token` = '".$token."';";
	$MQ = mysqli_query($GLOBALS['DB'], $SQL);
	
	if(mysqli_num_rows($MQ) == 1){ // Valid User
	
		$MFA = mysqli_fetch_array($MQ);
		$user_id = $MFA['id'];
		
		if(isset($_REQUEST['message'])){
			if($_REQUEST['message'] != ""){
				// Check if message exists!
				$src = $user_id; // Sender
				$dst = 0; // Global
				$msgs = mysqli_real_escape_string($GLOBALS['DB'], $_REQUEST['message']);
				$time = date("U");
			
				$SQL = "INSERT INTO `message` VALUES(NULL, $src, $dst, '".$msgs."', $time);";
				$MQ_Send = mysqli_query($GLOBALS['DB'], $SQL);
				$Sent_Success = true;
			}
		}
	
		
		
		
		$SQL = "SELECT * FROM `message` WHERE `id` > ".$last." LIMIT 0, 25";
		$MQ2 = mysqli_query($GLOBALS['DB'], $SQL);
		
		if(mysqli_num_rows($MQ2) > 0){
			while($MFA2 = mysqli_fetch_array($MQ2)){
				$uid = $MFA2['src'];
				$SQL = "SELECT * FROM `user` WHERE `id` = $uid";
				$MQ3 = mysqli_query($GLOBALS['DB'], $SQL);
				$MFA3 = mysqli_fetch_array($MQ3);
				$Message[] = array( "message_id" => $MFA2['id'],
									"sender_id" => $MFA2['src'],
									"sender_name" => $MFA3['name'],
									"message" => $MFA2['message'],
									"time" => $MFA2['time']
								   );
				$last_id = $MFA2['id'];
			}
			
			if(isset($Sent_Success)){
				echo "SENT_SUCCESS<:>".$last_id."<:>";
			}else{
				echo "NOT_SENT<:>".$last_id."<:>";
			}
			
			foreach($Message as $iMessage){
				echo $iMessage["sender_name"] . "\t <" . date("h:i:s a", $iMessage["time"]) . ">: " . $iMessage["message"] . "<:>";
			}
			
		}else{
			echo "NOTHING";
		}
	}else{
		// Invalid Request
		echo "FAILED<:>Error: Authentication Failed";
	}
			
}else{
	// Invalid Request
	echo "Error: Invalid Request";
}



?>