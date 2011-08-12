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
  $user_results = array();
  $company_results = array();
  $search_input = check_plain($str);
  
  $result = db_query("SELECT u.uid, roles.rid FROM {users} u INNER JOIN {realname} r ON u.uid = r.uid INNER JOIN {users_roles} roles ON roles.uid = u.uid WHERE ((UPPER(u.name) LIKE '%%%s%') OR (UPPER(r.realname) LIKE '%%%s%')) AND (roles.rid = %d OR roles.rid = %d) GROUP BY u.uid", strtoupper($search_input), strtoupper($search_input), APPLICANT_RID, COMPANY_RID);

  while ($row = db_fetch_object($result)) {
    if ($row->rid == APPLICANT_RID)
      $user_results[] = $row->uid;
    else {
      $company_results[] = $row->uid;
    }
  }
  
  // print company data
	if (!empty($company_results)) {
		$output .= t('<h3>Companies</h3>');
		foreach ($company_results as $uid) {
			//Build the node to output
			$user_profile = get_user_profile($uid);
      if (!empty($user_profile))
        $output .= node_view($user_profile, $teaser = true);
		}
	}

	// print applicant data
  if (!empty($user_results)) {
		if (!empty($company_results))
			$output .= t('<h3>Users</h3>');
		foreach ($user_results as $uid) {
			$user_profile = get_user_profile($uid);
      if (!empty($user_profile))
        $output .= node_view($user_profile, $teaser = true);
		}
	}
	
	if (empty($user_results) && empty($company_results))
		$output .= 'No user names match your search criteria.';
	
  $result = db_query("SELECT n.nid FROM {node} n WHERE n.type = '%s' AND LOWER(n.title) LIKE '%%%s%'", 'job_specification', strtolower($search_input));
  if (!empty($result)) {
    $jobs = array();
    while ($row = db_fetch_object($result)) {
      if (!empty($row->nid)) {
        $node = node_load($row->nid);
        $jobs[] = node_view($node, $teaser = true);
      }
    }
    if (sizeOf($jobs) > 0)
      $output .= t('<h3>Jobs</h3>').implode($jobs, '');
  }
  
	$output = t('<div class="user-search">').$output.'</div>';
  return $output;
}