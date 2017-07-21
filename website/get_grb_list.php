<?php


ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include 'config.php';

$event_type = $_GET['event_type'];

$config = getConfig();

$dbh = new PDO('mysql:host='.$config['host'].';dbname='.$config['database'].';port=3306',$config['username'],$config['password']);


$query = "SELECT * from grb g join lc_grb l on(g.id_grb = l.id_grb) join orbit o on(o.id_orbit = g.id_orbit)";

//echo $query;

$grb_list = array();

$rows = $dbh->query($query);

foreach ($rows  as $row)
{
    $id_grb = $row['id_grb'];
    $trigger_number = $row['trigger_number'];
    $t_zero = $row['t_zero'];
    $s_on_n = $row['s_on_n'];
    $background = $row['background'];
    $threshold = $row['threshold'];
    $counts = $row['counts'];
    $orbit_number = $row['number'];
    $trend = $row['trend'];
    $file_name = $row['file_name'];

    array_push($grb_list,array('id_grb'=>$id_grb,'trigger_number'=>$trigger_number,'t_zero'=>$t_zero,'s_on_n'=>$s_on_n,'background'=>$background,'threshold'=>$threshold,'counts'=>$counts,'orbit_number'=>$orbit_number,'trend'=>$trend,'file_name'=>$file_name));

}

echo json_encode($grb_list);

?>
