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
  $list_size = 3;
    
  //~ Create array of contacts
  $result = db_query_range('SELECT realname.name AS name, users_user_relationships.uid AS uid, users_user_relationships.picture AS picture
                        FROM {user_relationships} user_relationships
                        LEFT JOIN {users} users_user_relationships ON user_relationships.requestee_id = users_user_relationships.uid
                        LEFT JOIN {users} users_user_relationships_1 ON user_relationships.requester_id = users_user_relationships_1.uid
                        LEFT JOIN {user_relationship_types} user_relationship_types_user_relationships ON user_relationships.rtid = user_relationship_types_user_relationships.rtid
                        INNER JOIN (SELECT node.uid AS uid, uprofile.field_name_value AS name FROM {node} node INNER JOIN {content_type_uprofile} uprofile ON node.nid = uprofile.nid WHERE LOWER(uprofile.field_name_value) like LOWER("%%%s%"))
                         realname ON realname.uid = users_user_relationships.uid
                        WHERE (user_relationships.approved = "%s") AND (user_relationships.requester_id = %d)
                        ORDER BY realname.name ASC', $arg, $relationship_status, $current_user, 0, 10);
  $user_results = array();
  while ($row = db_fetch_object($result)) {
  
    //~ Get the right avatar image to display, depending on whether or not the user has uploaded a custom profile avatar or not
    //~ $base_path = base_path();
    $image_path = $row->picture;

    if (!$row->picture) {
    
      $result = db_query('SELECT nid FROM {node} n WHERE type = \'%s\' AND n.uid = %d LIMIT %d', 'uprofile', $row->uid, 1);
      $result = db_fetch_object($result);
      $user_profile = node_load($result->nid);

      if ($user_profile->field_gender[0]['value'] == 'Male')
        $image_path = path_to_theme().'/images/default-image-m.jpg';
      elseif ($user_profile->field_gender[0]['value'] == 'Female')
        $image_path = path_to_theme().'/images/default-image-f.jpg';
      else
        $image_path = path_to_theme().'/images/default-image.jpg';
    }
  
      /* Now fill the auto complete menu... */
      $list[] = array('value' => $row->uid, 'name' => truncate_utf8($row->name, 50, TRUE, TRUE), 'image' => theme('imagecache', 'thumb-mini', $image_path, '', ''));
  }
  return $list;
}