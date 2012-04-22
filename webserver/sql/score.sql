--
-- Table structure for table `score`
--

CREATE TABLE IF NOT EXISTS `score` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `red_score` int(11) NOT NULL,
  `blue_score` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `score`
--

INSERT INTO `score` (`id`, `red_score`, `blue_score`) VALUES
(1, 0, 0);
