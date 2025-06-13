<html>
<head>
	<meta HTTP-EQUIV='Content-Type' CONTENT='text/html; charset=UTF-8'>
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
//CREATE table SpeciesMain(
//cname VARCHAR(40),arithaiid int,youtubetext VARCHAR(256),youtubeid VARCHAR(40),vid VARCHAR(40));
//CREATE table SpeciesYoutube(
//cname VARCHAR(40),arithaino int,youtubeid VARCHAR(40),vid VARCHAR(40));
$conn = new mysqli($servername, $username, $password, $database);
if (!empty($cname)) {
  $sql = "select * from SpeciesMain where cname='$cname';";
  mysqli_set_charset($conn,"utf8");
  $result = $conn->query($sql);
//echo "$sql;num=".$result->num_rows."<br>";
  if ($result->num_rows > 0) {
  // output data of each row
    while($row = $result->fetch_assoc()) {
      echo $row["cname"]. "_".$row["arithaiid"]."_".$row["youtubetext"]."_".$row["youtubeid"]."_".$row["vid"]. "<br>";
      $cname       =  $row["cname"];
      $arithaino   =  $row["arithaiid"];
      $youtubetext =  $row["youtubetext"];
      $youtubeid   =  $row["youtubeid"];
      $vid         =  $row["vid"];
//  $sql2 = "update Species set vid='$cnt',youtubeid='咖啡' where cname='$cname';";
    }
  }
  $result = $conn->query($sql2);
//echo "Connected successfully 3\n";
}
else {
  $cname       = '咖啡';
  $arithaiid   = 18;
  $youtubetext = "咖啡葉薄革質，光滑深綠色，卵狀披針形，長6—14釐米，寬3.5—5釐米
界：植物界 Plantae
演化支：維管束植物 Tracheophyta
演化支：被子植物 Angiosperms
演化支：真雙子葉植物 Eudicots
演化支：菊類植物 Asterids
目：龍膽目 Gentianales
科：茜草科 Rubiaceae
亞科：仙丹花亞科 Ixoroideae
族：咖啡族 Coffeeae
屬：咖啡屬 Coffea
咖啡";
  $youtubeid   = 'BMdmGhR6ooI';
  $vid         = 'VID20250324112600';
}   
//echo "rows:".$result->num_rows,"<br>\n";
//echo "cname=$cname,ipc=$ipc<br>";
echo "
	<TITLE>$cname-$youtubeid-$vid</TITLE>
</head><body>
<a href=index.html>HOME</a>&nbsp;&nbsp;
<br><pre>$youtubetext</pre><br>";
if(!empty($youtubetext)) {
  $sql2 = "select * from SpeciesYoutube where cname='$cname' order by arithaino;";
  $result2 = $conn->query($sql2);
//echo "$sql;num=".$result->num_rows."<br>";
  if ($result2->num_rows > 0) {
  // output data of each row
    while($row2 = $result->fetch_assoc()) {
//    echo $row["cname"]. "_".$row["arithaiid"]."_".$row["youtubetext"]."_".$row["youtubeid"]."_".$row["vid"]. "<br>";
      $cname       =  $row2["cname"];
      $arithaino   =  $row2["arithaino"];
      $youtubeid   =  $row2["youtubeid"];
      $vid         =  $row2["vid"];
      echo "<iframe width='315' height='560' src='https://www.youtube.com/embed/$$youtubeid' ";
      echo "title='$vid'";
      echo "allow='accelerometer; autoplay; clipboard-write; 
                   encrypted-media; gyroscope; picture-in-picture; web-share'";
      echo "allowfullscreen></iframe><br>";              
    }
  }
  $result = $conn->query($sql2);
}
$conn->close();	
?>
</body>
</html>