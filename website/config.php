<?php

// copy this file to config.php and change the configurations without commiting the new file with git.

function getConfig()
{
  // db data
  $host = "localhost";
  $username = "root";
  $password = "freespirit2016";
  $database = "mcal";

  // Directory that contains DeepVar
  $directory = "/home/rt/mcal";

  // Directory where save the tmp data generated by the dashboard
  $mapData = "/home/rt/mcal/website/tmp";


  // rt-alert db path
  $rt_alert_db = "/home/rt/db/rt-alert.sqlite";

  $array = array('host'=>$host,'username' => $username,'password' => $password,'database' => $database,'data'=>$mapData,'directory'=>$directory,'rt-alert'=>$rt_alert_db);

  return $array;
}

?>
