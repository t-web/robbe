<?php
header('content-type:text/html;charset:utf-8');

$__text__ = '';
$__mode__ = 2;
$__timer__ = 0;
$_act = '';
if ( isset($_POST['_act']) ) {
	$_act = $_POST['_act'];
	if ( $_act == 'split'  ) {
		$__text__ = $_POST['text'];
		$__mode__ = intval( isset($_POST['mode'])?$_POST['mode']:1 );

		$s_time = timer();
		$_result = rb_split($__text__, $__mode__);
		$__timer__ = timer() - $s_time;
	}
}

function timer() {
	list($msec, $sec) = explode(' ', microtime());	
	return ((float)$msec + (float)$sec);
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>robbe分词测试程序</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<style type="text/css">
		#box {width: 1000px;}
		.input-text {border: 1px solid #CCC;width: 1000px;height: 200px;background-color: #FFF;
			color: #555;font-size: 14px;}
		.link-box {overflow: hidden;zoom:1;padding-top:10px;}
		#submit-link {float:right;width:150px;height: 26px;line-height: 26px;
			background-color: #A50100;color: #FFF;font-weight: bold;text-align: center;
			text-decoration: none;font-size: 14px;}
		#info-link {float:right;width:300px;height: 26px;line-height: 26px;
			background-color: #A50100;color: #FFF;font-weight: bold;text-align: center;
			text-decoration: none;font-size: 14px;}
		.link-item {float: left;font-size: 14px;font-weight: bold;
			height: 26px;line-height: 26px;width: 100px;color: #A50100;}
		.title-item {height:30px;line-height: 30px;font-size: 14px;font-weight: bold;}
	</style>
</head>

<body>
	<div id="box">
		<div class="title-item">请输入分词内容：</div>
		<form name="robbe" method="post" action="robbe.tst.php">
			<div class="r-item"><textarea name="text" class="input-text" id="text"><?php echo $__text__;?></textarea></div>
			<input type="hidden" name="_act" value="split"/>
			<div class="link-box">
			<a class="link-item">
				<input type="radio" name="mode" value="1" <?php echo $__mode__==1?'checked="checked"':'';?>/>简易模式</a>
			<a class="link-item">
				<input type="radio" name="mode" value="2" <?php echo $__mode__==2?'checked="checked"':'';?>/>复杂模式</a>
			<a href="javascript:;" onclick="do_submit();return false;" id="submit-link">robbe分词</a>
		</div>
		</form>

		<?php
		if ( $_act == 'split' ) {
		?>
		<div class="title-item">robbe分词结果：</div>
		<div><textarea class="input-text"><?php foreach ( $_result as $_value ) echo $_value.' ';?>
		</textarea></div>
		<div class="link-box"><a id="info-link">
			<?php
			$len = strlen($__text__);
			if ( $len >= 1048576 ) {
				echo substr(($len/1048576), 0, 6).'MB';
			} else if ( $len >= 1024 ) {
				echo substr( ($len / 1024), 0, 6).'KB';
			} else {
				echo $len.'B';
			}
			?>
			&nbsp;&nbsp;&nbsp;<?php printf("%.5f", $__timer__)?>sec
		</a></div>
		<?php
		}
		?>
	</div>

<script type="text/javascript">
String.prototype.trim = function() {return this.replace(/^\s+|\s+$/g, '');}
function do_submit() {
	var text = document.getElementById('text');
	if ( text.value.trim() == '' ) return; 
	document.robbe.submit();
}
</script>
</body>