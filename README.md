## libhash

[![Build Status](https://travis-ci.com/midoks/libhash.svg?branch=master)](https://travis-ci.com/midoks/libhash)


## Example

- Murmur3 algorithm 

```
<?php
echo \LibHash\Murmur3::to32("foo");
$v = \LibHash\Murmur3::to64("foo");

echo "\r\n";
echo $v[0];
echo "\r\n";
echo $v[1];
echo "\r\n";
?>
```

- Cityhash algorithm

```
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
```
