<?php 
$memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  if($memdata) {
    parse_str($memdata, $output);
    $cname = $output['cname'];
  } 
$ip = $_SERVER['REMOTE_ADDR'] ;
$ipc = strcmp(substr($ip,0,9),"192.168.0");
if ($ipc!=0) {
  include 'db.php';
}
else {
  include 'dbx.php';
}
$conn = new mysqli($servername, $username, $password, $database);
if (!empty($cname)) {
  $sql = "select * from Species where cname='$cname';";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
  // output data of each row
    while($row = $result->fetch_assoc()) {
    //echo $row["cname"]. "_".$row["vid"]. "_".$row["youtubeid"]."<br>";
      $cname     = $row["cname"];
      $vid       =   $row["vid"];
      $youtubeid =   $row["youtubeid"];
//  $sql2 = "update Species set vid='$cnt',youtubeid='咖啡' where cname='$cname';";
    }
  }
}
else {
  $cname = 'coffee';
  $youtubeid = 'BMdmGhR6ooI';
  $vid = 'VID20250324112600';
}   
//echo "rows:".$result->num_rows,"<br>\n";
//echo "cname=$cname,ipc=$ipc<br>";
echo "<html>
<head>
	<TITLE>$cname-$youtubeid-$vid</TITLE>
	<meta HTTP-EQUIV='Content-Type' CONTENT='text/html; charset=UTF-8'>
</head>";	
// Create connection
//$conn = new mysqli($servername, $username, $password, $database);

echo "in php 2\n";
// Check connection
if ($conn!=NULL) {
    echo "in php 4\n";
  if ($conn->connect_error) {
        echo "in php 5\n";
    die("Connection failed: " . $conn->connect_error);
  }
}
else {
  echo "in php 3\n";
}
$sql = "select * from Species;";
$result = $conn->query($sql);
echo "rows:".$result->num_rows,"<br>\n";
if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc()) {
    echo $row["cname"]. "_".$row["vid"]. "_".$row["youtubeid"]."<br>";
    $cname     = $row["cname"];
    $vid       =   $row["vid"];
    $youtubeid =   $row["vid"];
//  $sql2 = "update Species set vid='$cnt',youtubeid='咖啡' where cname='$cname';";
  }
} else {
  echo "0 results";
}
//mysql_free_result($result);
$result = $conn->query($sql2);
$conn->close();


echo "Connected successfully 3\n";
?>
Test2