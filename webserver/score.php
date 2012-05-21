<?php defined('VERSION') or die('No direct script access.');
include_once('db.php');
class  UpdateScore{

    private $team;
    private $score;
    
    public function __construct()
    {
        $this->db = new Db();
    }

    public function update($team,$score) 
    {
        $this->team = $team;
        $this->score = $score;
        if( $this->team AND is_numeric($this->score) )
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
