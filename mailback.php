<?php
$email = "arithaicom@gmail.com";
$subject =  "Email Test";
$message = "this is a mail testing email function on server";
$memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  if($memdata) {
    parse_str($memdata, $output);
    $code = $output['code'];
    $subject = $output['subject'];
    $message = $output['message'];
    $headers = 'From: sale@arithai.com'       . "\r\n" .
                 'Reply-To: sale@arithai.com' . "\r\n" .
                 'X-Mailer: PHP/' . phpversion();

    $headers = "MIME-Version: 1.0" . "\r\n";
    $headers .= "Content-type:text/html;charset=UTF-8" . "\r\n";

// More headers
    $headers .= 'From: <service@arithai.com>' . "\r\n";
    $headers .= 'Cc: service@arithai.com' . "\r\n";
//  mail($email,$subject,$message,$headers);
  } 
//$code="123456";
//echo "<hr>$code...$subject...$message<hr>";
if (!empty($code) &&  strcasecmp( $code, '123456' ) == 0 ) {
  $sendMail = mail($email, $subject, $message, $headers);
  if($sendMail)
  {
?>
<html>
	<head>
		<script>
			window.location.replace("http://arithai.com/");
		</script>
	</head>
</html>
<?php
//  echo "Email Sent Successfully";
  }
  else
  {
    echo "Mail Failed";
  }
}
else
{
?>




<?php
}
?>

