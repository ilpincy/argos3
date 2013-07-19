<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<?php include "php/utility.php"; ?>
<html>

  <head>
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,700,400italic" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Ubuntu+Mono:400,700,400italic" rel="stylesheet" type="text/css">
    <link rel="stylesheet" type="text/css" href="../css/style.css">
	  <script type="text/javascript" src="../media/flowplayer/flowplayer-3.2.6.min.js"></script>
    <title>The ARGoS Website</title>
    <script type="text/javascript">

      var _gaq = _gaq || [];
      _gaq.push(['_setAccount', 'UA-860753-4']);
      _gaq.push(['_trackPageview']);

      (function() {
      var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
      ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
      var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
      })();

    </script>
    <script type="text/javascript" src="jquery.js"></script>
    <script type="text/javascript" src="dynsections.js"></script>
    $treeview
    $search
    $mathjax
    <link href="api_embedded_style.css" rel="stylesheet" type="text/css" />
  </head>

  <body>

    <div id="everything">
      <div id="logo">
        <a href="index.php" target="_self">ARGoS</a><br>
          <span>Large-scale robot simulations</span>
        </div>
        <div class="menu">
          <ul>
            <li><a href="../index.php">home</a></li>
            <li><a href="../about.php">about</a></li>
            <li><a href="../download.php">download</a></li>
            <li style="border: 0"><a href="../documentation.php" class="current">documentation</a></li>
          </ul>
        </div>
        <br>
          <div class="menu">
            <ul>
              <li><a href="../examples.php">examples</a></li>
              <li><a href="../user_manual.php">user manual</a></li>
              <li><a href="../dev_manual.php">dev manual</a></li>
              <li><a href="../api/index.php" class="current">API</a></li>
              <li><a href="../forum.php">forum</a></li>
              <li style="border: 0"><a href="../publications.php">publications</a></li>
            </ul>
          </div>
          <div style="clear:both"></div>
          <div class="section" style="height: 100px; background-image: url(../img/povray_screenshot.png)"></div>

<!------------------------>
<!--   CONTENT STARTS   -->
<!------------------------>

<div class="section" id="doxygen">

<div id="top"><!-- do not remove this div, it is closed by doxygen! -->
  
  <!--BEGIN TITLEAREA-->
  <div id="titlearea">
    <table cellspacing="0" cellpadding="0">
      <tbody>
        <tr>
          <!--BEGIN DISABLE_INDEX-->
          <!--BEGIN SEARCHENGINE-->
          <td>$searchbox</td>
          <!--END SEARCHENGINE-->
          <!--END DISABLE_INDEX-->
        </tr>
      </tbody>
    </table>
  </div>
  <!--END TITLEAREA-->
  <!-- end header part -->
