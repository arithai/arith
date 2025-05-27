Test
<?php
$servername = "localhost";
$username = "root";
$password = "111111";
$database = "mysql";

echo "in php\n";
// Create connection
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
    echo $row["cname"]. "<br>";
  }
} else {
  echo "0 results";
}
mysql_free_result($result);
$conn->close();


echo "Connected successfully 3\n";
?>
Test2