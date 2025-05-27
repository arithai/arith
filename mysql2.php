<head>
	<TITLE> 葉綠素生技 </TITLE>
	<meta HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
</head>	
Test

<?php 
$ip = $_SERVER['REMOTE_ADDR'] ;
$ipc = strcmp(substr($ip,0,9),"192.168.0");
if ($ipc!=0) {
  include 'db.php';
}
else {
  include 'dbx.php';
}
echo "in php $ipc <br>\n";
// Create connection
//$conn = new mysqli($servername, $username, $password, $database);
$conn = new mysqli($servername, $username, $password, $database);
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
    $cname = $row["cname"];
    $cnt = 1+$row["vid"];
    $sql2 = "update Species set vid='$cnt',youtubeid='咖啡' where cname='$cname';";
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