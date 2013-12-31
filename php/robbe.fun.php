<?php
header("Content-Type:text/html;charset=utf-8;");

echo "constant access:<br />";
echo "complex mode: ".RB_CMODE.", simple mode: ".RB_SMODE."<br />";
echo "rb_ucode_utf8(20013)=".rb_ucode_utf8(20013)."<br />";
echo "rb_utf8_ucode(中)=".rb_utf8_ucode("中")."<br />";
echo "rb_utf8_bytes(中)=".rb_utf8_bytes("中")."<p />";

echo "词库函数：<br />";
echo "rb_dic_exist(研究) ? ".rb_dic_exist(RB_LEX_CJK, "研究")."<br />";

$_entry = rb_dic_get(RB_LEX_CJK, "你");
echo "rb_dic_get(你)：<br />";
echo "|——length: ".$_entry["length"].", freq: ".$_entry["freq"]."<p />";

//version about.
echo "friso_version(): ", friso_version(), "<br />";
echo "rb_version(): " , rb_version(), ", rb_charset(): ", rb_charset(), "<p />";

echo "分词函数：<br />";
if ( rb_charset() == 'UTF-8' )
{
	$_str = "研究生命起源,robbe高性能php中文分词组件。";
	echo "rb_split(\"" . $_str . "\")：<br />";
	
	//API:
	//rb_split(string, Array, [long])
	//1.string: 要被切分的字符串。
	//2.Array: 配置选项，使用NULL来选择默认的配置(friso.ini中的配置)。
	//3.long: 可选参数，自定义切分返回选项，查看下面的$_rargs
	
	//1.完整的配置: 
	//array('max_len'=>5, 'r_name'=>0, 'mix_len'=>2, 'lna_len'=>1, 'add_syn'=>1,
	//	'clr_stw'=>1, 'keep_urec'=>0, 'spx_out'=>0, 'en_sseg'=> 1, 'st_minl'=>2, 'kpuncs'=>'.+#', 'mode'=>RB_CMODE);
	//1.在不了解friso内核的情况下, 请不要随便更改nthreshold
	//2.使用NULL来使用php.ini中指定的friso.ini文件中的配置
	
	//2.返回选项：
	//词条: RB_RET_WORD, 类别：RB_RET_TYPE, 长度：RB_RET_LENGTH, 真实长度：RB_RET_RLEN, 偏移量：RB_RET_OFF
	//词性：RB_RET_POS(待实现)
	$_rargs = RB_RET_TYPE | RB_RET_LEN | RB_RET_RLEN | RB_RET_OFF | RB_RET_POS;
	//$_rargs = 0;
	
	//3.切分类别：
	//CJK词条：RB_TYP_CJK, 英中混合词(b超)：RB_TYP_ECM，中英混合词(卡拉ok)：RB_TYP_CEM，
	//英文标点混合词(c++)：RB_TYP_EPUN，标点：RB_TYP_PUN，未知类别：RB_TYP_UNK，其他类别(同义词)：RB_TYP_OTR
	$_result = rb_split($_str, array('mode'=>RB_CMODE), $_rargs);
	unset($_str);
	foreach ( $_result as $_val )
	{
		$_str = $_val['word'];
		if ( $_rargs != 0 ) {
			$_str .= '[';
			if ( ($_rargs & RB_RET_TYPE) != 0 )
				$_str .= ', type: '.$_val['type'];		//获取词条类别
			if ( ($_rargs * RB_RET_LEN) )
			$_str .= ', len: ' . $_val['len'];			//词条长度
			if ( ($_rargs & RB_RET_RLEN) != 0 )
			$_str .= ', rlen: ' . $_val['rlen'];		//词条真实长度
			if ( ($_rargs * RB_RET_OFF) != 0 )
			$_str .= ', off: ' . $_val['off'];			//词条偏移量
			if ( ($_rargs & RB_RET_POS) != 0 )
				$_str .= ', pos: ' . $_val['pos'];		//词条词性
			$_str .= ']';
		}

		$_str .= '/ ';
		echo $_str;
	}
}
else echo "set charset to UTF-8 to test function rb_split.";
?>
