function loadGRB(id_orbit)
{
  // start loader
  document.getElementById("loader").style.display="block";

  // ajax call

  $.ajax({ cache: false,
    url:'get_event_list.php',
    type:'get',
    data: {'event_type':0,'id_orbit':id_orbit},
    contentType: "application/json; charset=utf-8",
    async: false

  }).done(function (data) {

    console.log(data);
    var event_list = JSON.parse(data);

    document.getElementById('grb-table-row').style.display="block";
    document.getElementById("loader").style.display="none";

    var dataSet = new Array();

    for(var i = 0 ; i<event_list.length ; i++)
    {

      var id_grb,t_zero,s_on_n,background,threshold,counts,orbit_number;

      id_grb = event_list[i].id_grb;
      t_zero = event_list[i].t_zero;
      date = event_list[i].date;
      time = event_list[i].time;
      bin = event_list[i].bin;
      orbit_number = event_list[i].orbit_number;
      file_name = event_list[i].file_name;
      // add table data
      dataSet[i] = new Array();

      for (var j = 0; j <=5; j++) {

        if(j==0)
        {
          dataSet[i][j] =orbit_number;
        }
        if(j==1)
        {
          dataSet[i][j] =t_zero;
        }
        if(j==2)
        {
          dataSet[i][j] =date;
        }
        if(j==3)
        {
          dataSet[i][j] =time;
        }
        if(j==4)
        {
          dataSet[i][j] =bin;
        }
        if(j==5)
        {
          var link = "./grb_lc.php?file_name="+file_name+"&t_zero="+t_zero+"&orbit_number="+orbit_number;

          dataSet[i][j] = '<button type="submit" onclick="location.href=\''+link+'\'" return false;" class="btn btn-success" >Open Light Curve</button>';

        }
      }
    }
    if ( $.fn.dataTable.isDataTable( '#grb-table') ) // se la tabella esiste già
    {
      var table = new $.fn.dataTable.Api( '#grb-table' ).clear();
      table.rows.add(dataSet).draw();
    }
    else
    {
      console.log("create table");
      var table = $('#grb-table').DataTable( {

        columns: [
          { title:"Orbit Number"},
          { title: "T0" },
          { title: "Date" },
          { title: "Time" },
          { title: "Bin" },
          { title: "Light Curve" }


        ]
      } );
      table.rows.add(dataSet).draw();
    }



  }).fail(function (jqXHR, textStatus) {
    console.log("error");
    console.log(textStatus);
  });

}
function loadTGF(id_orbit)
{
  // start loader
  document.getElementById("loader").style.display="block";

  // ajax call

  $.ajax({ cache: false,
    url:'get_event_list.php',
    type:'get',
    data: {'event_type':2,'id_orbit':id_orbit},
    contentType: "application/json; charset=utf-8",
    async: false

  }).done(function (data) {

    console.log(data);
    var event_list = JSON.parse(data);

    document.getElementById('tgf-table-row').style.display="block";
    document.getElementById("loader").style.display="none";

    var dataSet = new Array();

    for(var i = 0 ; i<event_list.length ; i++)
    {

      var id_grb,t_zero,counts,orbit_number,date,time,hr,duration,e_max,e_avg,lon,lat,local_time;

      id_grb = event_list[i].id_grb;
      t_zero = event_list[i].t_zero;
      local_time = event_list[i].local_time;
      counts = event_list[i].counts;
      orbit_number = event_list[i].orbit_number;
      file_name = event_list[i].file_name;
      file_name_geo = event_list[i].file_name_geo;
      date = event_list[i].date;
      time = event_list[i].time;
      hr = event_list[i].hr;
      duration = event_list[i].duration;
      e_max = event_list[i].e_max;
      e_avg = event_list[i].e_avg;
      lon = event_list[i].lon;
      lat = event_list[i].lat;


      // add table data
      dataSet[i] = new Array();

      for (var j = 0; j <=12; j++) {

        if(j==0)
        {
          dataSet[i][j] =orbit_number;
        }
        if(j==1)
        {
          dataSet[i][j] =date;
        }
        if(j==2)
        {
          dataSet[i][j] =time;
        }
        if(j==3)
        {
          dataSet[i][j] =local_time;
        }
        if(j==4)
        {
          dataSet[i][j] =t_zero;
        }
        if(j==5)
        {
          dataSet[i][j] =counts;
        }
        if(j==6)
        {
          dataSet[i][j] =hr;
        }
        if(j==7)
        {
          dataSet[i][j] =duration;
        }
        if(j==8)
        {
          dataSet[i][j] =e_max;
        }
        if(j==9)
        {
          dataSet[i][j] =e_avg;
        }
        if(j==10)
        {
          dataSet[i][j] =lon;
        }
        if(j==11)
        {
          dataSet[i][j] =lat;
        }
        if(j==12)
        {
          var link = "./tgf_lc.php?file_name="+file_name+"&t_zero="+t_zero+"&date="+date+"&time="+time+"&orbit_number="+orbit_number;
          var link_geo = "./tgf_lc.php?file_name="+file_name_geo+"&t_zero="+t_zero+"&date="+date+"&time="+time+"&orbit_number="+orbit_number;

          dataSet[i][j] = '<button type="submit" onclick="location.href=\''+link+'\'" return false;" class="btn btn-success" >Open Light Curve</button> <button type="submit" onclick="location.href=\''+link_geo+'\'" return false;" class="btn btn-success" >Open Map</button>';

        }
      }
    }
    if ( $.fn.dataTable.isDataTable( '#tgf-table') ) // se la tabella esiste già
    {
      var table = new $.fn.dataTable.Api( '#tgf-table' ).clear();
      table.rows.add(dataSet).draw();
    }
    else
    {
      console.log("create table");
      var table = $('#tgf-table').DataTable( {

        columns: [
          { title:"Orbit Number"},
          { title:"Date"},
          { title:"Time"},
          { title: "T0" },
{ title: "Local Time" },
          { title: "Counts" },
          { title: "HR" },
          { title: "Duration" },
          { title:"E_MAX"},
          { title:"E_AVG"},
          { title:"Lon"},
          { title:"Lat"},
          { title: "Light Curve" }

        ]
      } );
      table.rows.add(dataSet).draw();
    }



  }).fail(function (jqXHR, textStatus) {
    console.log("error");
    console.log(textStatus);
  });

}
function loadSTE(id_orbit)
{
  // start loader
  document.getElementById("loader").style.display="block";

  // ajax call

  $.ajax({ cache: false,
    url:'get_event_list.php',
    type:'get',
    data: {'event_type':1,'id_orbit':id_orbit},
    contentType: "application/json; charset=utf-8",
    async: false

  }).done(function (data) {

    console.log(data);
    var event_list = JSON.parse(data);

    document.getElementById('ste-table-row').style.display="block";
    document.getElementById("loader").style.display="none";

    var dataSet = new Array();

    for(var i = 0 ; i<event_list.length ; i++)
    {

      var bin,id_ste,t_zero,s_on_n,background,threshold,counts,orbit_number,date,time;

      id_ste = event_list[i].id_ste;
      t_zero = event_list[i].t_zero;
      s_on_n = event_list[i].s_on_n;
      background = event_list[i].background;
      threshold = event_list[i].threshold;
      counts = event_list[i].counts;
      orbit_number = event_list[i].orbit_number;
      file_name = event_list[i].file_name;
      bin = event_list[i].bin;
      date = event_list[i].date;
      time = event_list[i].time;
      // add table data
      dataSet[i] = new Array();

      for (var j = 0; j <=9; j++) {

        if(j==0)
        {
          dataSet[i][j] =orbit_number;
        }
        if(j==1)
        {
          dataSet[i][j] =bin;
        }
        if(j==2)
        {
          dataSet[i][j] =s_on_n;
        }
        if(j==3)
        {
          dataSet[i][j] =t_zero;
        }
        if(j==4)
        {
          dataSet[i][j] =date;
        }
        if(j==5)
        {
          dataSet[i][j] =time;
        }
        if(j==6)
        {
          dataSet[i][j] =background;
        }
        if(j==7)
        {
          dataSet[i][j] =threshold;
        }
        if(j==8)
        {
          dataSet[i][j] =counts;
        }
        if(j==9)
        {
          var link = "./ste_lc.php?file_name="+file_name+"&t_zero="+t_zero+"&orbit_number="+orbit_number;

          dataSet[i][j] = '<button type="submit" onclick="location.href=\''+link+'\'" return false;" class="btn btn-success" >Open Light Curve</button>';

        }
      }
    }
    if ( $.fn.dataTable.isDataTable( '#ste-table') ) // se la tabella esiste già
    {
      var table = new $.fn.dataTable.Api( '#ste-table' ).clear();
      table.rows.add(dataSet).draw();
    }
    else
    {
      console.log("create table");
      var table = $('#ste-table').DataTable( {

        columns: [
          { title:"Orbit Number"},
          { title:"Bin"},
          { title: "S/N" },
          { title: "T0" },
          { title: "date" },
          { title: "time" },
          { title: "Background" },
          { title: "Threshold" },
          { title:"Counts"},
          { title: "Light Curve" }


        ]
      } );
      table.rows.add(dataSet).draw();
    }



  }).fail(function (jqXHR, textStatus) {
    console.log("error");
    console.log(textStatus);
  });

}
