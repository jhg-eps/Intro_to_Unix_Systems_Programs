# Joseph Garcia
# ECE 331 Sheaff
# Homework 10
# 4/7/15

# Worked extensively with Scott Edgerly on this assignment

1. 
$ sudo apt-get install libdbi-perl libdbd-sqlite3-perl

2. 
a. Yes
b. Yes
c. Yes
d. Yes
e. Not able to implement
My Public IP address is listed as 166.182.82.223, but it is not static and I cannot find a way to make it public. 

Source code:

<?php

// I do not take credit for the weeks_in_month function. It was written by Ultra_Master and posted at the below URL.
//www.experts-exchange.com/Programming/Languages/Scripting/PHP/Q_26846029.html
function weeks_in_month($month,$year)
{
    $firstday = date("w", mktime(0,0,0, $month, 1, $year));
    $lastday = date("t", mktime(0,0,0, $month, 1, $year));
    $count_weeks = 1 + ceil(($lastday-7+$firstday)/7);
    return $count_weeks;
}

$months=array('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'); // array of months
$one=1;
$date=new DateTime('now', new DateTimeZone('America/New_York'));   // Grab today's date, format unknown.
$month=$date->format('F');   // Full textual representation of the current month's na,e (e.g. March).
$dom=$date->format('j');     // day of the month without leading zeros
//$date->sub(new DateInterval('P' . $dom . 'D'));
$wd=$date->format('w');       // numeric representation of the day of the week, Sunday == 0 and Saturday == 6
//$date->sub(new DateInterval('P' . $wd . 'D'));
$year=date('Y');         // 4 digit format representing the year, e.g. 2014
$doy=date('z');          // grab the day of the year.
 // The month structures all look good now. 
// So now I need to find out what day of the week a month starts on....
//echo date('l', strtotime($month . " " . $year));  // find out day of 1st of month. 
//echo date('z');  // This number gives me the current day of the year. 
$day_value = 1;
$day_of_year=0;
for ($k=0;$k<12;$k++) {
    print "<TABLE BORDER=1>";
    print "<TR><TH COLSPAN=7>$months[$k]</TR>\n";
    print "<TR><TH>Sun<TH>Mon<TH>Tue<TH>Wed<TH>Thu<TH>Fri<TH>Sat</TR>";
    $month = $k+$one;   // number ID of month
    $weeks = weeks_in_month($month,$year);   // number of weeks in month
    $cal_day_of_month_start=date('w', strtotime($months[$k] . " " . $year)); // day index of the starting day of the month.
    $day_value = 1;     // reinitialize day 
    for ($i=1;$i<($weeks + 1);$i++) {
        if ($i == 1) {  // stuff to do for the first week.
            for($L = 0; $L < $cal_day_of_month_start; $L++) {
                print "<TD ALIGN=\"CENTER\"> ";
            }
            for ($L = $cal_day_of_month_start; $L < 07; $L++) {
                $day_of_year = $day_of_year + 1;
                if ($day_of_year == $doy) {
                    print "<TD ALIGN=\"CENTER\" BGCOLOR=\"#FFA000\">$day_value";
                }
                if ($day_of_year != $doy) {
                print "<TD ALIGN=\"CENTER\">$day_value";        // This covers the first row.
                }
                $day_value = $day_value + 1;
            }
            print "</TR>";
        }
        $days_in_month = cal_days_in_month(CAL_GREGORIAN, $month, $year);
        if (($i != 1) & (i != $weeks)) {
            for ($L = 0; ($L < 07 ) && ($day_value != ($days_in_month + 1)); $L++) {
                $day_of_year = $day_of_year + 1;
                if ($day_of_year == $doy) {
                    print "<TD ALIGN=\"CENTER\" BGCOLOR=\"FFA000\">$day_value";
                }
                if ($day_of_year != $doy) { 
                print "<TD ALIGN=\"CENTER\">$day_value";
                }
                $day_value = $day_value + 1;
            }
            for ($Lprime = $L; $Lprime < 07; $Lprime++) {
                print "<TD ALIGN=\"CENTER\"> ";
            }
            print "</TR>";
        }
    }
    print "</TABLE>\n";
}
?>
          
3. 
a.) Not able to work with Scott to create a working script.
b.) create table GoMoos(UTCtime text, watertempF real, waveheightft real, waveperiodsec real);  // create table
	.separator "  ";
	.import /home/joseph/Desktop/ECE_331_HW/gomoos.txt GoMoos
c. SELECT MAX(waveheightft) FROM GoMoos where rowid > 1; # assuming we want wave height and not the time at which it occurred.
   Numerical answer: Not Working
d. SELECT AVG(watertempF) FROM GoMoos where rowid > 1;
   Numerical answer: 29.044 degrees (which I suspect is incorrect).


