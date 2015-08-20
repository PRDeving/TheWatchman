<?php
    $myfile = fopen("servercall.txt", "w") or die("Unable to open file!");
    fwrite($myfile, $_GET['json']);
    fwrite($myfile,"");
    fclose($myfile);
?>
