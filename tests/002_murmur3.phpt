--TEST--
Check for libhash murmur3
--SKIPIF--
<?php if (!extension_loaded("libhash")) print "skip"; ?>
--FILE--
<?php
echo \LibHash\Murmur3::to32("foo");
$v = \LibHash\Murmur3::to64("foo");

echo "\r\n";
echo $v[0];
echo "\r\n";
echo $v[1];
echo "\r\n";
?>
--EXPECT--
-156908512
-2129773440516405919
9128664383759220103
