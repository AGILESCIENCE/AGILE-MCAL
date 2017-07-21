<!DOCTYPE html>
<html lang="en">

<head>

  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>AGILE GRB</title>

  <script src="js/jquery-3.0.0.min.js"></script>
  <script src="js/bootstrap.min.js"></script>

  <link rel="stylesheet" type="text/css" href="DataTables/dataTables.bootstrap.min.css">
  <link rel="stylesheet" type="text/css" href="DataTables/responsive.bootstrap.min.css">
  <script type="text/javascript" charset="utf8" src="DataTables/jquery.dataTables.min.js"></script>
  <script type="text/javascript" charset="utf8" src="DataTables/dataTables.bootstrap.min.js"></script>
  <script type="text/javascript" charset="utf8" src="DataTables/dataTables.responsive.min.js"></script>
<script type="text/javascript" charset="utf8" src="js/agilemcal.js"></script>
  <script type="text/javascript" charset="utf8" src="DataTables/responsive.bootstrap.min.js"></script>


  <!-- Bootstrap -->
  <link href="css/bootstrap.min.css" rel="stylesheet">

  <link rel="stylesheet" href="css/agilemcal.css">


  <script language = "javascript" type = "text/javascript">

  function loadData()
  {
    var show = "<?php echo $_GET['show'] ?>";
    var id_orbit = "<?php echo $_GET['id_orbit'] ?>";
    var orbit_number = "<?php echo $_GET['orbit_number'] ?>";
    console.log(orbit_number);

    if(show == 1)
    {
      document.getElementById('grb-table-row').style.display="none";
      document.getElementById('tgf-table-row').style.display="none";
      document.getElementById('ste-table-row').style.display="none";
      document.getElementById('grb_label').innerHTML="GRB for orbit "+orbit_number;
      loadGRB(id_orbit);

    }
    if(show == 2)
    {
      document.getElementById('grb-table-row').style.display="none";
      document.getElementById('tgf-table-row').style.display="none";
      document.getElementById('ste-table-row').style.display="none";
      document.getElementById('ste_label').innerHTML="STE for orbit "+orbit_number;
      loadSTE(id_orbit);
    }
    if(show == 3)
    {
      document.getElementById('grb-table-row').style.display="none";
      document.getElementById('tgf-table-row').style.display="none";
      document.getElementById('ste-table-row').style.display="none";

      document.getElementById('tgf_label').innerHTML="TGF for orbit "+orbit_number;
      loadTGF(id_orbit);


    }

  }


  </script>


</head>

<body onload="loadData()">


  <nav class="navbar navbar-default navbar-fixed-top" role="navigation">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navbar" >
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand" href="#">AGILE MCAL</a>
      </div>
      <div id="navbar" class="collapse navbar-collapse">
        <ul class="nav navbar-nav">
          <li class="active"><a href="index.html">Home</a></li>
          <li ><a href="grb.html">GRB</a></li>
          <li><a href="ste.html">STE</a></li>
          <li><a href="tgf.html">TGF</a></li>

        </ul>
      </div><!--/.nav-collapse -->
    </div>
  </nav>


  <div class="container-fluid">
    <div class="row" style="">

      <div class="col-lg-2 col-lg-offset-5 col-md-2 col-md-offset-5 col-sm-2 col-sm-offset-5 col-xs-2 col-xs-offset-5" id="loader" >

      </div>

    </div>
    <div class="row" id="grb-table-row" style="display:none;" >

      <h3 style="margin-left:15px;" id="grb_label">GRB List</h3>
      <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 " id="grb-table-div" >
        <table id="grb-table" class = "table table-striped  dt-responsive"> </table>

      </div>

    </div>
    <div class="row" id="ste-table-row" style="display:none;" >

      <h3 style="margin-left:15px;" id="ste_label">STE List</h3>
      <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 " id="ste-table-div" >
        <table id="ste-table" class = "table table-striped  dt-responsive"> </table>

      </div>

    </div>
    <div class="row" id="tgf-table-row" style="display:none;" >

      <h3 style="margin-left:15px;" id="tgf_label">TGF List</h3>
      <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 " id="tgf-table-div" >
        <table id="tgf-table" class = "table table-striped  dt-responsive" cellspacing="0" width="100%"> </table>

      </div>

    </div>
  </div>
</body>
</html>
