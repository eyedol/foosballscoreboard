<?php defined('VERSION') or die('No direct script access.');
define('FILENAME','config.ini');

/**
 * Handles database connection
 */
class Db {

    private $db;

    private $host;
    
    private $username;

    private $password;

    private $mysqli;

    private $config;

    public $config_info;

    public function __construct() 
    {
        try {
            $this->set_config(FILENAME);
            $this->config_info = $this->get_config();
            $this->db = $this->config_info['database']['name'];
            $this->host = $this->config_info['database']['host'];
            $this->username = $this->config_info['database']['username'];
            $this->password = $this->config_info['database']['password'];
            $this->init_connection();
            
        }catch(Exception $e){
            echo "File Not Found Exception:",$e->getMessage();
        }
    }

    private function init_connection()
    {
        $this->mysqli = new mysqli($this->host,$this->username,
            $this->password,$this->db);

        //throw error if there is one
        if ($this->mysqli->connect_errno) 
        {
            die("Failed to connect to MySQL: " . 
                $this->mysqli->connect_error);
        }

    }

    public function update($team,$score)
    {
        //update score for read team 
        if($team == 'r') {
            $sql = "UPDATE `score` SET `red_score` =? WHERE `id`=1";
        }
        else if($team == 'b' ){
            //update score for blue team
            $sql = "UPDATE `score` SET `blue_score` =? WHERE `id`=1";
        }

         $stmt = $this->mysqli->prepare($sql);
         $stmt->bind_param('s',$score);

         //execute statement
         if ($stmt->execute())
         { 
            return true;
         }

         $stmt->close(); 
         return false;
    }

    public function fetch_score()
    {
        //update score for read team 
        $sql = "SELECT `red_score`, `blue_score` FROM `score`";
        $stmt = $this->mysqli->query($sql);
        //execute statement
        $data = $stmt->fetch_object();
        $stmt->close(); 
        return $data;
    }


    public function set_config($config) 
    {
        if (file_exists($config)) 
        {
            $this->config = parse_ini_file($config, true);
        } else {
            throw new Exception("Please create a config.ini file by copying the content of config.template.ini");
        }
    }

    public function get_config()
    {
        return $this->config;
    }

}

?>
