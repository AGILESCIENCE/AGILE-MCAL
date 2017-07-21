<?php


ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include 'config.php';

$event_type = $_GET['event_type'];
$id_orbit = $_GET['id_orbit'];

$orbit_query = "";

if($id_orbit != 0)
  $orbit_query = "where o.id_orbit = ".$id_orbit;

$config = getConfig();

$dbh = new PDO('mysql:host='.$config['host'].';dbname='.$config['database'].';port=3306',$config['username'],$config['password']);

if($event_type==0) //grb
{
  $query = "SELECT * from grb g join lc_grb l on(g.id_grb = l.id_grb) join orbit o on(o.id_orbit = g.id_orbit) ".$orbit_query." order by t_zero desc";

  //echo $query;

  $grb_list = array();

  $rows = $dbh->query($query);

  foreach ($rows  as $row)
  {
    $id_grb = $row['id_grb'];
    $t_zero = $row['t_zero'];
    $date = $row['date'];
    $time = $row['time'];
    $bin = $row['bin'];
    $trend = $row['trend'];
    $file_name = $row['file_name'];
    $orbit_number = $row['orbit_number'];
    $trend = $row['trend'];

    array_push($grb_list,array('id_grb'=>$id_grb,'t_zero'=>$t_zero,'bin'=>$bin,'date'=>$date,'time'=>$time,'orbit_number'=>$orbit_number,'trend'=>$trend,'file_name'=>$file_name));

  }

  echo json_encode($grb_list);
}
if($event_type==1) //ste
{
  $query = "SELECT * from ste s join lc_ste l on(s.id_ste = l.id_ste) join orbit o on(o.id_orbit = s.id_orbit) ".$orbit_query." order by t_zero desc";

  //echo $query;

  $ste_list = array();

  $rows = $dbh->query($query);

  foreach ($rows  as $row)
  {
    $id_ste = $row['id_ste'];
    $t_zero = $row['t_zero'];
    $s_on_n = $row['s_on_n'];
    $background = $row['background'];
    $threshold = $row['threshold'];
    $counts = $row['counts'];
    $orbit_number = $row['orbit_number'];
    $trend = $row['trend'];
    $file_name = $row['file_name'];
    $bin = $row['bin'];
    $date = $row['date'];
    $time = $row['time'];

    array_push($ste_list,array('time'=>$time,'date'=>$date,'id_ste'=>$id_ste,'t_zero'=>$t_zero,'s_on_n'=>$s_on_n,'background'=>$background,'threshold'=>$threshold,'counts'=>$counts,'orbit_number'=>$orbit_number,'trend'=>$trend,'file_name'=>$file_name,'bin'=>$bin));

  }

  echo json_encode($ste_list);
}
if($event_type==2) //tgf
{
  $query = "SELECT o.trend,o.orbit_number,t.*,l.file_name,gt.file_name as 'file_name_geo' from tgf t join lc_tgf l on(t.id_tgf = l.id_tgf) join geo_tgf gt on(gt.id_tgf = t.id_tgf) join orbit o on(o.id_orbit = t.id_orbit) ".$orbit_query." order by t_zero desc";

  //echo $query;

  $tgf_list = array();

  $rows = $dbh->query($query);

  foreach ($rows  as $row)
  {
    $id_tgf = $row['id_tgf'];
    $t_zero = $row['t_zero'];
    $counts = $row['counts'];
    $orbit_number = $row['orbit_number'];
    $trend = $row['trend'];
    $file_name = $row['file_name'];
    $file_name_geo = $row['file_name_geo'];
    $local_time = $row['local_time'];
    $date = $row['date'];
    $time = $row['time'];
    $hr = $row['hr'];
    $duration = $row['duration'];
    $e_max = $row['e_max'];
    $e_avg = $row['e_avg'];
    $lon = $row['lon'];
    $lat = $row['lat'];

    array_push($tgf_list,array('file_name_geo'=>$file_name_geo,'local_time'=>$local_time,'id_tgf'=>$id_tgf,'duration'=>$duration,'e_max'=>$e_max,'e_avg'=>$e_avg,'lon'=>$lon,'lat'=>$lat,'date'=>$date,'time'=>$time,'hr'=>$hr,'t_zero'=>$t_zero,'counts'=>$counts,'orbit_number'=>$orbit_number,'trend'=>$trend,'file_name'=>$file_name));

  }

  echo json_encode($tgf_list);
}

?>
