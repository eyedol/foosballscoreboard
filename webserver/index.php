<?php
    define('VERSION',2);
    include_once('score.php');
    $update_score = new UpdateScore();
    if (isset($_GET['score']) && isset($_GET['team']))
    {
        $score = $_GET['score'];
        $team = $_GET['team'];

        $score = $update_score->update($team,$score);

    }else {
        $score = $update_score->scores(); 
    }

?>
<!DOCTYPE HTML>
<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=windows-1250">
        <meta name="author" content="ihub foosball">
        <title>iHub Foosball</title>
        <link rel="stylesheet" href="css/style.css"/>
    </head>
<body>
    <div class="round current" id="round1">
        <h1>Round 1</h1>
        <div class="p1Score"><?php print $score->red_score; ?></div>
        <div class="p2Score"><?php print $score->blue_score; ?></div>
    </div>
    <div class="p1Name">Red Team</div>
    <div class="p2Name">Blue Team</div>
</body>
