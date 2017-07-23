<?php


ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include 'config.php';

$event_type = $_GET['event_type'];

$config = getConfig();

$dbh = new PDO('mysql:host='.$config['host'].';dbname='.$config['database'].';port=3306',$config['username'],$config['password']);

// get orbit lcfirst

$query = "SELECT * from orbit order by orbit_number desc";

//echo $query;

$orbit_list = array();

$rows = $dbh->query($query);

foreach ($rows  as $row) // for each orbit
{
  $id_orbit = $row['id_orbit'];
  $orbit_number = $row['orbit_number'];
  $trend = $row['trend'];

  // get tgf Number

  $query_tgf = "select count(*) as 'number' from tgf where id_orbit = ".$id_orbit;
  $tgf_number = 0;
  foreach ($dbh->query($query_tgf)  as $row_tgf) // for each orbit
  {
    $tgf_number = $row_tgf['number'];
  }

  //get ste Number

  $query_ste = "select count(*) as 'number' from ste where id_orbit = ".$id_orbit;
  $ste_number = 0;
  foreach ($dbh->query($query_ste)  as $row_ste) // for each orbit
  {
    $ste_number = $row_ste['number'];
  }

  // get grb Number

  $query_grb = "select count(*) as 'number' from grb where id_orbit = ".$id_orbit;
  $grb_number = 0;
  foreach ($dbh->query($query_grb)  as $row_grb) // for each orbit
  {
    $grb_number = $row_grb['number'];
  }

  array_push($orbit_list,array('id_orbit'=>$id_orbit,'orbit_number'=>$orbit_number,'trend'=>$trend,'tgf_number'=>$tgf_number,'ste_number'=>$ste_number,'grb_number'=>$grb_number));

}

echo json_encode($orbit_list);


?>
