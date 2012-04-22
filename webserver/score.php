<?php defined('VERSION') or die('No direct script access.');
include_once('db.php');
class  UpdateScore{

    private $team;
    private $score;
    
    public function __construct($team,$score)
    {
        $this->team = $team;
        $this->score = $score;
        $this->db = new Db();
        $this->update();
    }

    public function update() 
    {
        if( $this->team AND $this->score)
        {
            //update score 
            $this->db->update($this->team, $this->score);
        }
    }

    public function scores() 
    {
        return $this->db->fetch_score();
    }

}

?>
