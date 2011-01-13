<?php
// $Id$

/**
 * @file
 * Search page callbacks for the usersearch module.
 *
 * @ingroup usersearch
 */
/**
* Callback function for the search results page
**/
function _user_search_results($str) {
  $output = '';
  $user_results;
  $company_results;
  $search_input = check_plain($str);
  
  $result = db_query('SELECT uid FROM {users} WHERE UPPER(name) LIKE "%%%s%"', strtoupper($search_input));

  while ($row = db_fetch_object($result)) {
    $user_tmp = user_load($row->uid);
    if (!(in_array($row->uid, $user_results)) && in_array('company', array_values($user_tmp->roles)))
      $company_results[] = $row->uid;
    else {
      if (!(in_array($row->uid, $user_results)) && in_array('applicant', array_values($user_tmp->roles)))
        $user_results[] = $row->uid;
    }
  }
  
  $result = db_query('SELECT node.uid as uid 
                      FROM {node} node
                      INNER JOIN {content_type_uprofile} uprofile ON node.nid = uprofile.nid
                      WHERE UPPER(uprofile.field_name_value) LIKE "%%%s%"', strtoupper($search_input));
                      
  while ($row = db_fetch_object($result)) {
    $user_tmp = user_load($row->uid);
    if (!(in_array($row->uid, $user_results)) && in_array('applicant', array_values($user_tmp->roles)))
      $user_results[] = $row->uid;
  }
  
  // print company data
	if (!empty($company_results)) {
		$output .= t('<h3>Companies</h3>');
		foreach ($company_results as $user_result) {
			$user_tmp = user_load($user_result);
			//Build the node to output
			$user_profile = get_user_profile($user_tmp->uid);
			$output .= node_view($user_profile, $teaser = true);
		}
	}

	// print applicant data
  if (!empty($user_results)) {
		if (!empty($company_results))
			$output .= t('<h3>Users</h3>');
		foreach ($user_results as $user_result) {
			$user_tmp = user_load($user_result);
			$user_profile = get_user_profile($user_tmp->uid);
			$output .= node_view($user_profile, $teaser = true);
		}
	}
	
	if (empty($user_results) && empty($company_results))
		$output .= 'No users or companies match your search criteria.';
	
	$output = t('<div class="user-search">').$output.'</div>';
  
  return $output;
}