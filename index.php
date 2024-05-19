<?php
// 取得資料 data.txt
//print "<hr color=red>";
//Revision 2010.04.14 directory structure change
//  c:\ccdsm\bin\treenet\index,php
//  c:\ccdsm\bin\treenet\sample1\00.htm 01.htm,... *.jpg
//  c:\ccdsm\treenet\sample1\data.txt , *.jpg
  $memdata=$_SERVER['QUERY_STRING']; //getenv("Query_String");
//print "<hr color=red>";
  if($memdata) {
    parse_str($memdata, $output);
    $udir = $output['udir'];
    $ID1 = $output['ID1']; 
    $u = $output['u'];
    $fun = $output['fun'];
    $txt_title = $output['txt_title'];
    $txt_description = $output['txt_description'];
    $txt_body = $output['txt_body'];
    $p = $output['p'];
  }  

  $isman=0;
  $fdata="./treenet/pass.txt";
  $lines = file($fdata);
  foreach ($lines as $line_num => $line) {
    if ($line_num >= 1) {
      $NAM[$line_num] = ltrim(trim(substr($line, 9, 12)));  //UNAME
      $NAP[$line_num] = ltrim(trim(substr($line,21, 15)));  //PASSWORD
      $NAC[$line_num] = ltrim(trim(substr($line,76,  8)));  //CNAME
//    print $u.")".$NAM[$line_num].">".$p."<".$NAP[$line_num];
      if($u==$NAM[$line_num] && $p==$NAP[$line_num]) {
        $isman=1;
        $udir=$NAC[$line_num];
      } 
    }
  }

  if(empty($udir)) $udir ="sample1";
  $fdata="./".$udir."/data.txt";
  $lines = file($fdata);
// Loop through our array, show HTML source as HTML source; and line numbers too.
  foreach ($lines as $line_num => $line) {
//  echo "Line #<b>{$line_num}</b> : " . htmlspecialchars($line) . "<br />\n";
//  echo substr($line, 0,  3)."<br>";  //3+3+3+20+20
//  echo substr($line, 3,  3)."<br>";
//  echo substr($line, 6,  3)."<br>";
//  echo substr($line, 9, 20)."<br>";
//  echo substr($line,29, 20)."<br>";
//  echo substr($line,49,200)."<hr>";
    if ($line_num >= 1) {
      $IDA[$line_num] = substr($line, 0,  3);  //ID1
      $IDB[$line_num] = substr($line, 3,  3);  //ID2
      $IDC[$line_num] = substr($line, 6,  3);  //Class
      $IDT[$line_num] = ltrim(trim(substr($line, 9,  20)));  //Title
      $IDD[$line_num] = ltrim(trim(substr($line,29,  20)));  //Description
      $IDO[$line_num] = ltrim(trim(substr($line,49,1800)));  //Body
    }  
  }
//2010.4.21 change to post
  if (empty($txt_title))        $txt_title=$_POST['txt_title'];
  if (empty($txt_description))  $txt_description=$_POST['txt_description'];
  if (empty($txt_body))         $txt_body=$_POST['txt_body'];
  $txt_body = str_replace("\r\n","<br>",$txt_body); //2010.04.14
//print "<hr color=red>txt_title=$txt_title<hr>";
  $IDN = $line_num;
  $IDMAX=0;
  for ($ii=1;$ii<=$IDN;$ii++) {
//  print "$IDA[$ii] ....<hr>";
    if ($IDMAX < $IDA[$line_num]) $IDMAX=$IDA[$line_num];
  }	
//$IDMAX 最大 ID
  $IDMAXA=$IDMAX+1001;
  $IDMAXA=substr($IDMAXA,1);
//print "<hr color=red>ID1=$ID1<hr>";
  $ID1=$ID1+1000;
//print "<hr color=red>ID1=$ID1<hr>";
  $ID1=substr($ID1,1);
//print "IDMAXA=$IDMAXA";
  if($fun=="addafter") {
    $fp = fopen($fdata, "w");
    $fdata=sprintf("data.txt  ID1(3), ID2(3), CLID(3), Title(20), Description(20), Body(20)\n");
//  echo "fdata=".$fdata."<hr>";
    fwrite($fp, $fdata);
    for ($ii=1;$ii<=$IDN;$ii++) {
      $fdata=sprintf("%3s%3s%3s%20s%20s%1800s\n", $IDA[$ii], $IDB[$ii], 
            $IDC[$ii], $IDT[$ii], $IDD[$ii], $IDO[$ii]);
      fwrite($fp, $fdata);
//    print "line .... $ii input ...<br>\n";
    }
    $fdata=sprintf("%3s%3s   %20s%20s%1800s",$IDMAXA,$ID1,$txt_title,$txt_description,$txt_body);
    fwrite($fp, $fdata);
    $IDMAX=$IDMAXA;
    $IDN++;
    $IDA[$IDN] = $IDMAXA;
    $IDB[$IDN] = $ID1;
    $IDC[$IDN] = "";
    $IDT[$IDN] = $txt_title;
    $IDD[$IDN] = $txt_description;   
    $IDO[$IDN] = $txt_body;   
//  print "<hr>title=$title<hr>";
    fclose($fp);
  }	
  $nn=0;
  $nnx=1000;
//print "<hr color=red>ID1=$ID1<hr>";
  for ($ii=1;$ii<=$IDN;$ii++) {
    if ($ID1==$IDA[$ii]) {
      $IDUP       =substr(1000+$IDB[$ii],1); 	
      $title      =$IDT[$ii];
      $description=$IDD[$ii];
      $body       =$IDO[$ii];
      $nnx        = $ii;
    }  
    if ($ID1==$IDB[$ii]) $nn++;
  }

  if($fun=="delafter" && $ID1 > 0) { // 沒有第一筆
    $fp = fopen($fdata, "w");
    $fdata=sprintf("data.txt  ID1(3), ID2(3), CLID(3), Title(20), Description(20), Body(20)\n");
    fwrite($fp, $fdata);
    for ($ii=1;$ii<=$IDN;$ii++) {
      if($nnx < 1000 && $ii == $nnx) {
      	$IDB[$ii] = 999;
      }        
      $fdata=sprintf("%3s%3s%3s%20s%20s%1800s\n", $IDA[$ii], $IDB[$ii], 
              $IDC[$ii], $IDT[$ii], $IDD[$ii], $IDO[$ii]);  
      fwrite($fp, $fdata);       
    }          
    fclose($fp);
    $ID1 = $IDUP;
    $nn=0;
    $nnx=1000;
    for ($ii=1;$ii<=$IDN;$ii++) {
      if ($ID1==$IDA[$ii]) {
        $IDUP       =substr(1000+$IDB[$ii],1); 	
        $title      =$IDT[$ii];
        $description=$IDD[$ii];
        $body       =$IDO[$ii];
        $nnx        = $ii;
      }  
      if ($ID1==$IDB[$ii]) $nn++;
    }      
  }	
  if($fun=="manafter") { // 沒有第一筆
    $fp = fopen($fdata, "w");
    $fdata=sprintf("data.txt  ID1(3), ID2(3), CLID(3), Title(20), Description(20), Body(20)\n");
//  echo "fdata=".$fdata."<hr>";
    fwrite($fp, $fdata);
//  echo "fdata=".$fdata."<hr>";
    for ($ii=1;$ii<=$IDN;$ii++) {
      if($nnx < 1000 && $ii == $nnx) {
        $IDT[$ii] = $txt_title;      
        $IDD[$ii] = $txt_description;      
        $IDO[$ii] = $txt_body;      
        $title       = $txt_title;      
        $description = $txt_description;      
        $body        = $txt_body;      
//      echo "<<<<<<<<<<<<< $txt_title >>>>>>>>>>>>>>>><br>";
      }        
      $fdata=sprintf("%3s%3s%3s%20s%20s%1800s\n", $IDA[$ii], $IDB[$ii], 
            $IDC[$ii], $IDT[$ii], $IDD[$ii], $IDO[$ii]);  
      fwrite($fp, $fdata);     
//    echo "fdata=".$fdata."<hr>";
    }        
    fclose($fp);
  }	
?>  
<?php
  function genPage($IDpage,$isGen) {
    global $u,$isman,$p,$IDN,$udir,$nn,$IDA,$IDB,$IDT,$IDD,$IDO;  
    $nn=0;
    for ($ii=1;$ii<=$IDN;$ii++) {
      if (substr(1000+$IDpage,1)==$IDA[$ii]) $IDUP=substr(1000+$IDB[$ii],1); 	
      if (substr(1000+$IDpage,1)==$IDB[$ii]) $nn++;
    }
    if($IDUP=="000") $IDUP="index";
    $IDii=$IDpage+1;  
    if($isGen==1) {
      $udira=".";
      $fdata="<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01//EN' 'http://www.w3.org/tr/html4/strict.dtd'>
<html lang=zh-tw>
<!-- start body --> 
<head>
<meta http-equiv='Content-Type' content='text/html; charset=big5' />
<meta http-equiv='expires' content='-1'>
<title>".$IDT[$IDii].",屋價網,無價網,物價網,五價網,開價,訂價,出價,售價,成交價,成本價,2024
</title>
</head>
<body background=".$udira."/back0.gif>".$_SERVER['SERVER_NAME']."<table cellspacing=0 cellpadding=0 width='100%' height='100%' border=0>
<td colspan=2 align=center bgcolor=#AAFFFF><font size=+2>
".$IDD[$IDii]."<br><!--樹網(tree net) ---  description 可變!   ---->
<hr><tr>";
    }
    else { 
      $udira=$udir; 
      $fdata="<tr>";    
    }
    if ($nn==0) {
      $fdata.="<td colspan=2 width='85%' height=200 ";
      $fdata.="background=".$udira."/back1.gif "; 
      $fdata.="bgcolor=#FFEEFF align=center><font color=blue>"; //$IDO[$IDii]";

if (strpos($_SERVER['HTTP_USER_AGENT'],"MSIE")) {
      $fdata.="<embed height=120 type=application/x-shockwave-flash width=450 src=http://js1.bloggerads.net/bloggerads_w1.swf?blogid=20120109000042&fid=5&allowscriptaccess='never'></embed>";
}
else 
{
      $fdata.="<script type='text/javascript' src='http://js1.bloggerads.net/showbanner.aspx?blogid=20120109000042&amp;charset=utf-8'></script>";
}
      $fdata.="
<!-- Counter Code START --><a href='http://www.e-zeeinternet.com/' target='_blank'>
<img src='http://www.e-zeeinternet.com/count.php?page=1081173&amp;style=default&amp;nbdigits=5' alt='HTML Hit Counter' border='0' />
</a><a href='http://www.e-zeeinternet.com/' title='HTML Hit Counter' target='_blank' 
style='font-family: Geneva, Arial, Helvetica, sans-serif; font-size: 10px; color: #000000; text-decoration: none;'>HTML Hit Counter
</a><!-- Counter Code END --><img src=a5.gif alt='Guest Book' border=0>
<a href='/page/guestbook.php'>-討論區<font size=+3>GuestBook</a></a><br>"
;

      if($isGen==1) { 
        $fdata.=$IDO[$IDii]."  <!----  body 可變!   ---->\n";
      }
      else { 
        $fdata.=str_replace("src=","src=".$udira."/",
                $IDO[$IDii])."  <!----  body 可變!   ---->\n";
      }
    }
    else {  	
      for ($ii=1;$ii<=$IDN;$ii++) {
        if (substr($IDpage+1000,1)==$IDB[$ii]) break;
      }
      $nny = 0; $hvv = (int) (80/$nn);
      $fdata.="<td rowspan=".($nn)." width='75%' ";
      if ($isman && ($fun=='add' || $fun=='man' || $fun=='del')) {
        $fdata.="height='100%' ";
      }
      else {
        $fdata.="height='80%'";
      }
      $fdata.="valign=middle align=center bgcolor=#FFEEFF";
//    if ($ID1==0) { //2010.04.14
        $fdata.=" background=".$udira."/back1.gif ";
//    }  
      $fdata.="><font color=#001122>\n"; 

if (strpos($_SERVER['HTTP_USER_AGENT'],"MSIE")) {
      $fdata.="<embed height=120 type=application/x-shockwave-flash width=450 src=http://js1.bloggerads.net/bloggerads_w1.swf?blogid=20120109000042&fid=5&allowscriptaccess='never'></embed>";
}
else 
{
      $fdata.="<script type='text/javascript' src='http://js1.bloggerads.net/showbanner.aspx?blogid=20120109000042&amp;charset=utf-8'></script>";
}

      $fdata.="
<!-- Counter Code START --><a href='http://www.e-zeeinternet.com/' target='_blank'>
<img src='http://www.e-zeeinternet.com/count.php?page=1154517&style=default&nbdigits=5&reloads=1' alt='Free Counter' border='0' >
</a><br><a href='http://www.e-zeeinternet.com/' title='Free Counter' target='_blank' 
style='font-family: Geneva, Arial, Helvetica, sans-serif; font-size: 10px; color: #000000; text-decoration: none;'>
Free Counter</a><!-- Counter Code END -->
&nbsp;
<img src=a5.gif alt='Guest Book' border=0>
<a href='/page/guestbook.php'>-討論區GuestBook</a><br>"
;

      if($isGen==1) { 
        $fdata.=$IDO[$IDii]."  <!----  body 可變!   ---->\n";
      }
      else { 
        $fdata.=str_replace("src=","src=".$udira."/",
                $IDO[$IDii])."  <!----  body 可變!   ---->\n";
      }

      $fdata.="</td><td width='15%' bgcolor=#AAFFFF height='".$hvv."%'><a href=";
      if($isGen==1) {
        if($ii==1) $fdata.="index.htm";
        else       $fdata.=substr($ii+999,1).".htm";
      }
      else {
        $fdata.="index.php?";
        if ($isman) $fdata.= "u=$u&p=$p&";
        $fdata.="ID1=".$IDA[$ii]."&udir=".$udir;
      }
      $fdata.="><font size=+1>".$IDT[$ii]."</a></td></tr>\n";  

      for ($kk=$ii+1;$kk<=$IDN;$kk++) {
        if ($IDpage==$IDB[$kk]) {	
          $fdata.="<tr><td width='15%' bgcolor=";
          $nny++;
          if ($nny & 1) {$fdata.= "#AABBFF";} else {$fdata.= "#AAFFFF";}
          $fdata.=" height='".$hvv."%'><a href=";
          if($isGen==1) {
            if($kk==1) $fdata.="index.htm";
            else       $fdata.=substr($kk+999,1).".htm";
          }
          else {
            $fdata.="index.php?";
            if ($isman) $fdata.= "u=$u&p=$p&";
            $fdata.="ID1=".$IDA[$kk]."&udir=".$udir;
          }
          $fdata.="><font size=+1>".$IDT[$kk]."</a></td></tr><!----  title 可變!   ---->\n";
        }  
      }
    }  
    if($isGen==1) {
      $fdata.="</td></tr><tr><td colspan=2 align=right>\n";
      $fdata.="<a href=".$IDUP.".htm>上目錄</a>&nbsp;&nbsp;&nbsp;\n";
      $fdata.="<a href=index.htm>首頁</a>&nbsp;&nbsp;&nbsp;\n";
      $fdata.="</table></body></html>\n";
    }
    return $fdata;
  } //genPage
?>
<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01//EN' 'http://www.w3.org/tr/html4/strict.dtd'>
<html lang=zh-tw>
<!-- start body --> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=big5" />
<meta http-equiv="expires" content="-1">
<title>
<?php
print $title;
?>,屋價網,無價網,物價網,五價網,開價,訂價,出價,售價,成交價,成本價,2024
</title>
</head>
<body background=<?php print $udir."/back0.gif" ?>
<a href=https://youtu.be/k1HZZykRExs>最新資訊2024.04.15[</a><?php echo $_SERVER['SERVER_NAME']; ?>][
<?php echo $_SERVER['HTTP_HOST']; ?>]

<div id="sfcs5julx2kdxaf3xay4t9kwy8zj2se7yl7"></div> 
<script type="text/javascript" src="https://counter8.optistats.ovh/private/counter.js?c=s5julx2kdxaf3xay4t9kwy8zj2se7yl7&down=async" async>
</script> 

<table cellspacing=0 cellpadding=0 width='100%' height='100%' border=0>
<td colspan=2 align=center bgcolor=#AAFFFF><font size=+2>

<?php
print $description;
?>,<a href=#>房屋出租</a>,<a href=#>房屋出售</a><br><!--樹網(tree net) ---  description 可變!   ---->
<hr> 
<?php
function upload_back() { 
  global $globals,$isShow,$isShowStr,$udir; 
  $isShow=true;
  $isShowStr="";
  $uploaddir = "./".$udir."/"; 
  $dir = opendir($uploaddir); 
  $isExist=false;
//$files = array(); 
  while($file = readdir($dir)) { 
    if(($file!='.' && $file!='..')) { 
//    array_push($files,"$file"); 
  //  if($isShow) $isShowStr = $isShowStr . "\n".
//       "<a href=".$udir."/".$file.">".$udir."/".$file."</a><br>";
    }
    if($file==$_FILES['attachement']['name'])
      $isExist=true;
  }
  closedir($dir); 
  $file=$_FILES['attachement']['name'];
  if(!empty($_POST['attachement_loos'])) { 
    $globals['attachement'] = $_POST['attachement_loos']; 
  }
  if(isset($_FILES['attachement']) && !empty($_FILES['attachement']['name'])) { 
    echo "Size of ".$_FILES['attachement']['name']." is ".$_FILES['attachement']['size']." bytes. <br>\n";
    if ($_FILES['attachement']['size'] > 300000) {
      echo "The uploaded file is large!<br>\n"; 
    }
    else 
    if(!eregi('image/', $_FILES['attachement']['type']) &&
       !eregi('audio/', $_FILES['attachement']['type']) &&
       !eregi('text/', $_FILES['attachement']['type'])
    ) { 
      echo "The uploaded file is not accepted.<br>\n"; //$_FILES['attachement']['type']; 
    } else { 
      //echo "file".$file." ...".$_FILES['attachement']['name']."<br>\n"; 
      if($isExist) {
        echo "The name of the uploaded file is duplicate!<br>\n"; 
      }
      else {
      //$_FILES['attachement']['name'] = ceil(count($files)+'1').''.strrchr($_FILES['attachement']['name'], '.'); 
      //$uploadfile = $uploaddir . basename($_FILES['attachement']['name']); 
        $uploadfile = $uploaddir . basename($file); 
        $previousToDestroy = empty($globals['attachement']) && !empty($file) ? '' : $uploaddir . $files[ceil(count($files)-'1')]; 
        if(!move_uploaded_file($_FILES['attachement']['tmp_name'], $uploadfile)) { 
echo '<pre> 
Your file was not uploaded please try again 
here are your debug informations: 
'.print_r($_FILES) .' 
</pre>'; 
        } else { 
//        echo "Image succesfully uploaded!<br>\n"; 
          if($isShow)
            $isShowStr = $isShowStr . "\n".
                         "<a href=".$udir."/".$file.">".$udir."/".$file."</a><br>";
        } 
        $globals['attachement'] = $file; 
        if(!empty($previousToDestroy)) { unlink($previousToDestroy); } 
      }
    } 
  } 
} 
//if($_POST['fun']=="upload") { echo "exit"; exit; }
if ($isman && $fun=='upload') {
  echo "upload back";
  upload_back(); 
  if($isShow) echo $isShowStr;
}
if ($isman && ($fun=='add' || $fun=='man' || $fun=='del')) {
  print "<tr><td colspan=2 align=center bgcolor=#AAEEFF>\n";
  if($fun=='add') {
    print "新增";
  }
  else if($fun=='man') {
    print "修改";
  }	
  else if($fun=='del') {
    print "刪除";
  }	  
  print "</td></tr>\n";

  print "<tr><td colspan=2 align=left>\n";
  echo "<center><table><form action='index.php?ID1=$ID1&u=$u&p=$p&fun=upload&udir=$udir' method='post' enctype='multipart/form-data'>    
    <input type='file'   name='attachement' ></input>
    <input type='hidden' name='attachement_loos' value='", $globals['attachement'] ,"'></input>
    <input type='hidden' name='fun' value='upload'></input>
    <input type='submit' value='上傳圖片'></input>
  </table></form></center>\n"; 

  print "<tr><td colspan=2 align=left>\n";
  print "<form method=post action=index.php?ID1=$ID1&u=$u&p=$p&fun=";
  if($fun=='add') {
    print "addafter>\n";
    print "<input type=hidden name=u   value='$u'>\n";
    print "<input type=hidden name=ID1 value='$ID1'>\n";
    print "<input type=hidden name=fun value='addafter'>\n";
  }
  else if($fun=='man') {
    print "manafter>\n";
    print "<input type=hidden name=u   value='$u'>\n";
    print "<input type=hidden name=ID1 value='$ID1'>\n";
    print "<input type=hidden name=fun value='manafter'>\n";
  }	
  else if($fun=='del') {
    print "delafter>\n";
    print "<input type=hidden name=u   value='$u'>\n";
    print "<input type=hidden name=ID1 value='$ID1'>\n";
    print "<input type=hidden name=fun value='delafter'>\n";
  }	
  else 
    print ">\n";
  print "連接抬頭=<input type=text name='txt_title' size=20 value='";
  if($fun=='man' || $fun=='del') {
    print $title;
  }	
  print "'  maxlength=20><br>\n";
  print "抬頭描述=<input type=text name='txt_description'  value='";
  if($fun=='man' || $fun=='del') {
    print $description;
  }	
  print "' size=20 maxlength=20><br>\n";

  print "表身描述=<textarea cols=100 rows=22 name='txt_body'>\n";
//print "表身描述=<input type=text name='txt_body' size=80  value='";  
  if($fun=='man' || $fun=='del') {
    print str_replace("<br>","\n",$body);
  }	
  print "</textarea><br>\n";
//print "' maxlength=200><br>\n";

  print "<input type='submit' value='確定'>\n";
  print "</form>";
  printf("</td></tr>\n");
}	
if ($isman && $fun=='gen') {
  print "<tr><td colspan=2 align=center bgcolor=#FF0000>\n";
  print "<a href=".$udir."/>生成網頁</a>";
  for ($ii=1;$ii<=$IDN;$ii++) {
    if($ii==1) {
      $fdata=$udir."/index.htm";
    }
    else {
      $fdata=$udir."/".substr($ii+999,1).".htm"; 
    }
    $fp = fopen($fdata, "w");
    $fdata=genPage($ii-1,1);
    fwrite($fp, $fdata);
    fclose($fp);  
  }
}
$fdata=genPage($ID1,0);
echo $fdata;
?>
</tr>
<tr>
<td colspan=2 align=right>
<?php
if ($ID1 > 0) {
  print "<a href=index.php?";
  if ($isman) print "u=$u&p=$p&";
  print "ID1=".$IDUP."&udir=$udir>上目錄</a>&nbsp;&nbsp;&nbsp;\n";
  print "<a href=index.php?";
  if ($isman) print "u=$u&p=$p&";
  print "udir=$udir>首頁</a>&nbsp;&nbsp;&nbsp;\n";
}  
if ( $isman) {
  if($ID1==0)
    print "<a href=index.php?u=$u&p=$p&udir=$udir&ID1=$ID1&fun=gen>生成網頁</a>&nbsp;&nbsp;&nbsp;\n";
  print "<a href=index.php?u=$u&p=$p&udir=$udir&ID1=$ID1&fun=man>維護本頁</a>&nbsp;&nbsp;&nbsp;\n";
  if ($ID1 > 0) {
    print "<a href=index.php?u=$u&p=$p&udir=$udir&ID1=$ID1&fun=del>刪除本頁</a>&nbsp;&nbsp;&nbsp;\n";
  }  
  print "<a href=index.php?u=$u&p=$p&udir=$udir&ID1=$ID1&fun=add>新增子頁</a>\n";
}	
?>
</td></tr>
</table>

<!-- hitwebcounter Code START -->
<a href="http://www.hitwebcounter.com" target="_blank">
<img src="http://hitwebcounter.com/counter/counter.php?page=6568402&style=0017&nbdigits=6&type=page&initCount=35791" title="" Alt=""   border="0" >
</a>                                        <br/>
                                        <!-- hitwebcounter.com --><a href="http://www.hitwebcounter.com" title="How Many People Visit" 
                                        target="_blank" style="font-family: Geneva, Arial, Helvetica, sans-serif; 
                                        font-size: 9px; color: #908C86; text-decoration: underline ;"><strong>How Many People Visit                                        </strong>
                                        </a>
<noscript>
<a href="https://www.freecounterstat.com" title="visitors counter">
<img src="https://counter8.optistats.ovh/private/freecounterstat.php?c=s5julx2kdxaf3xay4t9kwy8zj2se7yl7" 
border="0" title="visitors counter" alt="visitors counter"></a>
</noscript> 
<?php
  include 'index.html';
?>  
</body>
</html>
