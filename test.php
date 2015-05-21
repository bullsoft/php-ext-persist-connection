<?php
$a = new FixApi();
var_dump($a->isConnected());
var_dump($a->getConnection());
$a->connect("127.0.0.1@1234/tcp", "test", "test", true);
var_dump($a->isConnected());
var_dump($a->getConnection());
