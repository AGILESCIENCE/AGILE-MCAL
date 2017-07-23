<!DOCTYPE html>
<html lang="en">

<head>

  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>AGILE Orbit Trend</title>


  <script src="js/jquery-3.0.0.min.js"></script>
  <script src="js/bootstrap.min.js"></script>

  <link rel="stylesheet" type="text/css" href="DataTables/dataTables.bootstrap.min.css">
  <link rel="stylesheet" type="text/css" href="DataTables/responsive.bootstrap.min.css">
  <script type="text/javascript" charset="utf8" src="DataTables/jquery.dataTables.min.js"></script>
  <script type="text/javascript" charset="utf8" src="DataTables/dataTables.bootstrap.min.js"></script>
  <script type="text/javascript" charset="utf8" src="DataTables/dataTables.responsive.min.js"></script>

  <script type="text/javascript" charset="utf8" src="DataTables/responsive.bootstrap.min.js"></script>


  <!-- Bootstrap -->
  <link href="css/bootstrap.min.css" rel="stylesheet">

  <link rel="stylesheet" href="css/agilemcal.css">


  <script language = "javascript" type = "text/javascript">

  function loadData()
  {



    var orbit_number = "<?php echo $_GET['orbit_number'] ?>";
    var link_trigger =  "../mcaldata/"+orbit_number+"/"+orbit_number+"_triggers.txt";

    $.ajax({
      url : link_trigger,
      dataType: "text",
      success : function (data) {
        console.log(data);
        $("#text").html(data.replace( /\n/g,"<br>" ));
      }
    });

    document.getElementById("label").innerHTML="Orbit Number "+orbit_number;
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
          <li ><a href="ste.html">STE</a></li>
          <li><a href="tgf.html">TGF</a></li>

        </ul>
      </div><!--/.nav-collapse -->
    </div>
  </nav>




  <div class="container-fluid">

    <h3 id="label"></h3>
    <br>

    <div class="row" >

      <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12" id="text" >

      </div>

    </div>



  </div>
</body>
</html>
