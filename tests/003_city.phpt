--TEST--
Check for libhash cityhash
--SKIPIF--
<?php if (!extension_loaded("libhash")) print "skip"; ?>
--FILE--
<?php
echo \LibHash\City::to32("abc");
echo "\r\n";

echo \LibHash\City::to64("abc");

$v = \LibHash\City::to128("abc");

echo "\r\n";
echo $v[0];
echo "\r\n";
echo $v[1];
echo "\r\n";
?>
--EXPECT--
795041479
2640714258260161385
4143508125394299908
-6879828354153669051
