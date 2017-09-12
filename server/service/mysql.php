<?php

$host = "localhost";
$user = "root";
$pass = "PASSWORD_HERE";
$data = "chatdb";

$GLOBALS['DB'] = mysqli_connect($host, $user, $pass, $data);

?>