<?php

//Maximum file size. You may increase or decrease.
$MAX_SIZE = 2000000;
                            
//Allowable file ext. names. you may add more extension names.            
$FILE_EXTS  = array('.jpeg','.jpg','.png');                    

$site_name = $_SERVER['HTTP_HOST'];
$url_dir = "http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF']);
$url_this =  "http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'];

$upload_dir = "../tmp/";
$upload_url = $url_dir."/";
$message ="";

/************************************************************
 *     Process User's Request
 ************************************************************/

if ($_FILES['userfile']) {
  $resource = fopen("log.txt","a");
  fwrite($resource,date("Ymd h:i:s")." UPLOAD - {$_SERVER[REMOTE_ADDR]} {$_FILES['userfile']['name']} {$_FILES['userfile']['type']} \n");
  fclose($resource);

  $file_type = $_FILES['userfile']['type']; 
  $file_name = $_FILES['userfile']['name'];
  $file_ext = strtolower(substr($file_name,strrpos($file_name,".")));

  //File Size Check
  if ( $_FILES['userfile']['size'] > $MAX_SIZE) 
     $message = "The file size is over 2MB.";
  //File Extension Check
  else if (!in_array($file_ext, $FILE_EXTS))
     $message = "Sorry, $file_name($file_type) is not allowed to be uploaded.";
  else
     $message = do_upload($upload_dir, $upload_url);
}
else if (!$_FILES['userfile']);
else 
	$message = "Invalid File Specified.";

function do_upload($upload_dir, $upload_url) {

	$temp_name = $_FILES['userfile']['tmp_name'];
	$file_name = $_FILES['userfile']['name']; 
	$file_name = str_replace("\\","",$file_name);
  	$file_name = str_replace("'","",$file_name);
	$file_path = $upload_dir.$file_name;

	//File Name Check
  	if ( $file_name =="") { 
  		$message = "Invalid File Name Specified";
  		return $message;
  	}

  	$result  =  move_uploaded_file($temp_name, $file_path);
  	if (!chmod($file_path,0777)) {
   		$message = "change permission to 777 failed.";
   	}
  	else {
    	$message = ($result)?"$file_name uploaded successfully." : "Something went wrong.";
  		return $message;
  	}

}

?>

<html>
<head>
	<title>Logo Upload</title>
</head>

<body>

<?php echo $message; ?>

<form name="upload" id="upload" ENCTYPE="multipart/form-data" method="post">
	<input type="file" id="userfile" name="userfile">
    <input type="submit" name="upload" value="Upload">
</form>

</body>
</html>
