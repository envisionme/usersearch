<?php
// $Id$
/**
 * @file
 * 	file handling the json process
 */
 
/**
 * Callback function for the json page
 */
function usersearch_autosuggest() {
  $user_results;
  $company_results;
  $list = array();
  $arg = $_GET['as'];
  $list = _usersearch_get_contacts($arg);
  
  header("Content-type: application/json");
  echo json_encode($list);
  exit();
}

/**
 * Function calculating the results to send via ajax
 */
function _usersearch_get_contacts($arg) {
  $list = array();
  global $user;
  $current_user = $user->uid;
  $relationship_status = '1';

	$result = db_query_range('SELECT r.realname AS name, r.uid AS uid, u.picture AS picture FROM {realname} r INNER JOIN {user_relationships} rl ON r.uid = rl.requester_id INNER JOIN {users} u ON r.uid = u.uid WHERE rl.approved = 1 AND LOWER(r.realname) LIKE  "%%%s%" GROUP BY r.uid', strtolower($arg), 0, 10);
  while ($row = db_fetch_object($result)) {
  
    //~ Get the right avatar image to display, depending on whether or not the user has uploaded a custom profile avatar or not
    //~ $base_path = base_path();
    $image_path = $row->picture;
    /*if (!$row->picture) {
    
      $result = db_query('SELECT nid FROM {node} n WHERE type = \'%s\' AND n.uid = %d LIMIT %d', 'uprofile', $row->uid, 1);
      $result = db_fetch_object($result);
      $user_profile = node_load($result->nid);

      if ($user_profile->field_gender[0]['value'] == 'Male')
        $image_path = path_to_theme().'/images/default-image-m.jpg';
      elseif ($user_profile->field_gender[0]['value'] == 'Female')
        $image_path = path_to_theme().'/images/default-image-f.jpg';
      else
        $image_path = path_to_theme().'/images/default-image.jpg';
    }/**/
  
      /* Now fill the auto complete menu... */
     $list[] = array('value' => $row->uid, 'name' => truncate_utf8($row->name, 50, TRUE, TRUE), 'image' => theme('imagecache', 'thumb-mini', $image_path, '', ''));
  }
  return $list;
}