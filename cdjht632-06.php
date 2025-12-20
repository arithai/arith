<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
  <meta http-equiv="content-type" content="text/html; charset=utf-8">
  <meta name="generator" content="PSPad editor, www.pspad.com">
  <title></title>
<style>
.textfield{
	font-family:"細明體";
	font-size:14pt;
	height:26px;
}
.textfield3{
	font-family:"細明體";
	font-size:14pt;
	height:26px;
}
</style>
</head>
<body>
<canvas id="myCanvas" width="300" height="300">
</canvas>
<div id='mytext'><br>
<script>
  var x = 150;
  var y = 150;
  var radius = 100;
  var dx = 2;
  var dy = 2;
  var canvas = document.getElementById('myCanvas');
  var ctx = canvas.getContext('2d');
  function draw() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.beginPath();

    ctx.lineWidth = 2;
    var x1;
    var y1;
/*
    for(x1 = 0; x1 < canvas.width-1; x1 = x1+10) {
      ctx.moveTo(x1, 1);
      ctx.lineTo(x1, canvas.height-1);
    }
    for(y1 = 0; y1 < canvas.height-1; y1 = y1+10) {
      ctx.moveTo(1, y1);
      ctx.lineTo(canvas.width-1, y1);
    }
*/  
    ctx.arc(x, y, radius, 0, 2 * Math.PI);
    ctx.fillStyle = 'red';
    ctx.fill();
    ctx.stroke();
    x += dx;
    y += dy;
    if(x + radius > canvas.width || x - radius < 0) {
        dx = -dx;
    }
    if(y + radius > canvas.height || y - radius < 0) {
        dy = -dy;
    }
    requestAnimationFrame(draw);
  }
  draw();
  var N=100000;
  var myv="Given N="+N+",<br>";
  var x0;
  var y0;
  var ymax=Math.trunc(Math.sqrt(N));
  var sum=0
  for (x0=0;x0<=ymax;x0++) {
    for (y0=ymax;y0>=0;y0--) {
       if(x0*x0+y0*y0<N) {
 //      myv=myv+"("+x0.toString()+","+y0.toString()+"),<br>";
         if(x0==0) { 
           if(y0==0) { 
             sum++; 
           }
           else
            sum+=2;
         }
         else {
          if(y0==0) { 
            sum+=2;
          }
          else {
           sum+=4;
          }
        }    
      } 
    }
  } 
  myv=myv+"sum="+sum.toString()+"<br>";
  document.getElementById('mytext').innerHTML = myv;
</script>
<form method="POST">
</div>
<input name="action" type="Hidden">	
<input name="r" type="Hidden" value="G">
<input type=text size=20 maxlength=20>
</form>
</body>
</html>


<?php
/*
header("Content-type: image/xbm");
$memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  $mytext = 'Hello';
  if($memdata) {
    parse_str($memdata, $output);
    $mytext = $output[$mytext];
  } 

$fontsize = 5;
$wide = imagefontwidth($fontsize) * strlen($mytext) + 20;
$temp1=$wide/8;
$temp2=round($wide/8);
if ($temp1<>$temp2){$wide=($temp2 * 8) +8;}
$high = imagefontheight($fontsize) + 20;
$picture = imagecreatetruecolor($wide,$high);
$white = imagecolorallocate($picture,255,255,255);
imagefill($picture,0,0,$white);
$black=imagecolorallocate($picture, 0, 0, 0);
imageline($picture, 0, 0, 0, $high, $black);
imageline($picture, 0, 0, $wide, 0, $black);
imageline($picture, $wide-1, 0, $wide-1, $high-1, $black);
imageline($picture, 0, $high-1, $wide-1, $high-1, $black);
imagestring($picture,$fontsize,10,10,$mytext,$black);
imagexbm($picture); //This function is only available if PHP is compiled with the bundled version of the GD library
imagexbm($picture,"mytextpic.xbm");
imagedestroy($picture);
*/
?>
