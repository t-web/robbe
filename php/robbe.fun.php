<?php
header("Content-Type:text/html;charset=utf-8;");

echo "constant access:<br />";
echo "complex mode: ".__RB_COMPLEX_MODE__.", simple mode: ".__RB_SIMPLE_MODE__."<br />";
echo "rb_ucode_utf8(20013)=".rb_ucode_utf8(20013)."<br />";
echo "rb_utf8_ucode(中)=".rb_utf8_ucode("中")."<br />";
echo "rb_utf8_bytes(中)=".rb_utf8_bytes("中")."<br /><br />";

echo "词库函数：<br />";
echo "rb_dic_exist(研究) ? ".rb_dic_exist(__RB_LEX_CJK_WORDS__, "研究")."<br />";

$_entry = rb_dic_get(__RB_LEX_CJK_WORDS__, "你");
echo "rb_dic_get(你)：<br />";
echo "|--length: ".$_entry["length"].", freq: ".$_entry["freq"]."<br /><br />";

echo "分词函数：<br />";
$__str__ = "robbe高性能php中文分词组件。";
echo "rb_split(" . $__str__ . ")：<br />";
$_result = rb_split($__str__, __RB_COMPLEX_MODE__);
foreach ( $_result as $_value ) {
	echo $_value."/ ";
}
?>
