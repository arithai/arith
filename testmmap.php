<?php
print "mmap"; 
//$myfile = fopen("sh.txt", "a") or die("Unable to open file!");
$myfile = mmap_open('sh.txt', 128, 0);
echo fread($myfile,filesize("sh.txt"));
$write="wwwww";
fwrite($myfile,$write);
fclose($myfile);
?>
