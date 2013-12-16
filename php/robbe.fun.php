<?php
header("Content-Type:text/html;charset=utf-8;");

echo "constant access:<br />";
echo "complex mode: ".__RB_COMPLEX_MODE__.", simple mode: ".__RB_SIMPLE_MODE__."<br />";
echo "rb_ucode_utf8(20013)=".rb_ucode_utf8(20013)."<br />";
echo "rb_utf8_ucode(中)=".rb_utf8_ucode("中")."<br />";
echo "rb_utf8_bytes(中)=".rb_utf8_bytes("中")."<p />";

echo "词库函数：<br />";
echo "rb_dic_exist(研究) ? ".rb_dic_exist(__RB_LEX_CJK_WORDS__, "研究")."<br />";

$_entry = rb_dic_get(__RB_LEX_CJK_WORDS__, "你");
echo "rb_dic_get(你)：<br />";
echo "|——length: ".$_entry["length"].", freq: ".$_entry["freq"]."<p />";

//version about.
echo "friso_version(): ", friso_version(), "<br />";
echo "rb_version(): " , rb_version(), ", rb_charset(): ", rb_charset(), "<p />";

echo "分词函数：<br />";
$__str__ = "研究生命起源,robbe高性能php中文分词组件。";
echo "rb_split(\"" . $__str__ . "\")：<br />";
//完整的配置: 
//array('max_len'=>5, 'r_name'=>0, 'mix_len'=>2, 'lna_len'=>1, 'add_syn'=>1, 'clr_stw'=>1, 'keep_urec'=>0, 'spx_out'=>0, 'nthreshold'=>1000000, 'mode'=>__RB_COMPLEX_MODE__);
//1.在不了解friso内核的情况下, 请不要随便更改nthreshold
//2.使用NULL来使用php.ini中指定的friso.ini文件中的配置
$_result = rb_split($__str__, array('mode'=>__RB_COMPLEX_MODE__, 'spx_out'=>1));
foreach ( $_result as $_value ) {
	echo $_value."/ ";
}
?>
