<?php
//To save/get cache that easily just save this as cache.php or whatever you see fit:
function save_cache($data, $name, $timeout) {
    // delete cache
    $id=shmop_open(get_cache_id($name), "a", 0, 0);
    shmop_delete($id);
    shmop_close($id);
    
    // get id for name of cache
    $id=shmop_open(get_cache_id($name), "c", 0644, strlen(serialize($data)));
    
    // return int for data size or boolean false for fail
    if ($id) {
        set_timeout($name, $timeout);
        return shmop_write($id, serialize($data), 0);
    }
    else return false;
}

function get_cache($name) {
    if (!check_timeout($name)) {
        $id=shmop_open(get_cache_id($name), "a", 0, 0);

        if ($id) $data=unserialize(shmop_read($id, 0, shmop_size($id)));
        else return false;          // failed to load data

        if ($data) {                // array retrieved
            shmop_close();
            return $data;
        }
        else return false;          // failed to load data
    }
    else return false;              // data was expired
}

function get_cache_id($name) {
    // maintain list of caches here
    $id=array(  'test1' => 1,
                'test2' => 2
                );

    return $id[$name];
}

function set_timeout($name, $int) {
    $timeout=new DateTime(date('Y-m-d H:i:s'));
    date_add($timeout, date_interval_create_from_date_string("$int seconds"));
    $timeout=date_format($timeout, 'YmdHis');

    $id=shmop_open(100, "a", 0, 0);
    if ($id) $tl=unserialize(shmop_read($id, 0, shmop_size($id)));
    else $tl=array();
    shmop_delete($id);
    shmop_close($id);

    $tl[$name]=$timeout;
    $id=shmop_open(100, "c", 0644, strlen(serialize($tl)));
    shmop_write($id, serialize($tl), 0);
}

function check_timeout($name) {
    $now=new DateTime(date('Y-m-d H:i:s'));
    $now=date_format($now, 'YmdHis');

    $id=shmop_open(100, "a", 0, 0);
    if ($id) $tl=unserialize(shmop_read($id, 0, shmop_size($id)));
    else return true;
    shmop_close($id);

    $timeout=$tl[$name];
    return (intval($now)>intval($timeout));
}

echo "<html><head></head><pre>";
//shmop made simple:
// Create 100 byte shared memory block with system id of 0xff3
$shm_id = shmop_open(0xff3, "c", 0644, 100);
if (!$shm_id) {
    echo "Couldn't create shared memory segment\n";
}

// Get shared memory block's size
$shm_size = shmop_size($shm_id);
echo "SHM Block Size: " . $shm_size . " has been created.\n";

// Lets write a test string into shared memory
$shm_bytes_written = shmop_write($shm_id, "my shared memory block test", 0);
if ($shm_bytes_written != strlen("my shared memory block test")) {
    echo "Couldn't write the entire length of data<br>\n";
}

// Now lets read the string back
$my_string = shmop_read($shm_id, 0, $shm_size);
if (!$my_string) {
    echo "Couldn't read from shared memory block<br>\n";
}
echo "The data inside shared memory was: " . $my_string . "<br>\n";

//Now lets delete the block and close the shared memory segment
//if (!shmop_delete($shm_id)) {
//    echo "Couldn't mark shared memory block for deletion.<br>";
//}
shmop_close($shm_id);
?>
