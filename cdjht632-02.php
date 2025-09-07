<html>
<head>
<script type="text/javascript">
   document.cookie = 'window_width='+window.innerWidth+'; expires=Fri, 3 Aug 2901 20:47:11 UTC; path=/';
</script>
	<meta HTTP-EQUIV='Content-Type' CONTENT='text/html; charset=UTF-8'>
<?php 
$memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  if($memdata) {
    parse_str($memdata, $output);
    $cname = $output['cname'];
    $programtext = $output['program'];
  } 
  else {
    $cname = "cd1234.php";
    $programtext = '<?php
$a=1;
$b=-2;
if ($a!=0) {
  $x=-$b/$a;
  echo "ax+b=0 解得 x=".$x;
}
else {
  echo "a 不能為0";
}
?>';
  }
$ip = $_SERVER['REMOTE_ADDR'] ;
$ipc = strcmp(substr($ip,0,9),"192.168.0");
if ($ipc!=0) {
//include 'db.php';
}
else {
//include 'dbx.php';
}
$sprogramtext=htmlentities($programtext, ENT_QUOTES);
echo "
	<TITLE>$cname</TITLE>
</head><body>
<a href=cdjht632-02.php>HOME</a>&nbsp;&nbsp;
<br><pre>$sprogramtext</pre><br>";
if(!empty($programtext) && substr($cname,0,2)=='cd') {
  $fp = fopen($cname.".php", "w");
  fwrite($fp, $programtext);
  fclose($fp);
}
?>
<form method=GET action=cdjht632-02.php>student id:
<input type=text maxlength=20 name=cname value='cd1234'><br>
<textarea cols=60 rows=15 name=program>
<?php
  echo "$sprogramtext";
?>
</textarea><br>
<input type=submit value='執行'></form><br>
Result:
<hr>
<?php
  if (!empty($cname)) {
    include $cname.'.php';
  }
?>
</body>
</html>

