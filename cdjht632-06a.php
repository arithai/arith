<html>
<head>
</head>
<body>

<?php
  $memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  $mytext = 'Hello';
  if($memdata) {
    parse_str($memdata, $output);
    $mytext = $output[$mytext];
  } 

if(isset($mytext)){print '<img src="cdjht632-06.php?mytext=' . $mytext . '">';}

?>

<table width="654" border="1" align="center" cellpadding="3" cellspacing="0" bgcolor="#ffffff">
<tr>
<td align='center'><b><h2>Enter Some Text</h2></b></td>
</tr>
</table>
<table width="650" border="1" align="center" cellpadding="0" cellspacing="1" bgcolor="#ffffff">
<tr>
<form name="form" method="post" action="mytext-convert-to-xbm-image.php">
<td>
<table width="650" border="0" cellpadding="3" cellspacing="1" bgcolor="#ffffff">
<tr>
<td><b>Enter text to convert to image</b></td>
</tr>
<tr>
<td><input name="mytext" type="text" size="100" maxlength="100"></td>
</tr>
<tr>
<td><input type="submit" name="Submit" value="Submit"> <input type="reset" name="Submit" value="Reset"></td>
</tr>
</table>
</td>
</form>
</tr>
</table>
</body>
</html>
