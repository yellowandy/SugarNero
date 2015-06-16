<?php
    if(!defined('sugarEntry') || !sugarEntry) die('Not A Valid Entry Point');
    /*********************************************************************************
     * The contents of this file are subject to the SugarCRM Professional End User
     * License Agreement ("License") which can be viewed at
     * http://www.sugarcrm.com/crm/products/sugar-professional-eula.html
     * By installing or using this file, You have unconditionally agreed to the
     * terms and conditions of the License, and You may not use this file except in
     * compliance with the License.  Under the terms of the license, You shall not,
     * among other things: 1) sublicense, resell, rent, lease, redistribute, assign
     * or otherwise transfer Your rights to the Software, and 2) use the Software
     * for timesharing or service bureau purposes such as hosting the Software for
     * commercial gain and/or for the benefit of a third party.  Use of the Software
     * may be subject to applicable fees and any use of the Software without first
     * paying applicable fees is strictly prohibited.  You do not have the right to
     * remove SugarCRM copyrights from the source code or user interface.
     *
     * All copies of the Covered Code must include on each user interface screen:
     *  (i) the "Powered by SugarCRM" logo and
     *  (ii) the SugarCRM copyright notice
     * in the same form as they appear in the distribution.  See full license for
     * requirements.
     *
     * Your Warranty, Limitations of liability and Indemnity are expressly stated
     * in the License.  Please refer to the License for the specific language
     * governing these rights and limitations under the License.  Portions created
     * by SugarCRM are Copyright (C) 2004-2006 SugarCRM, Inc.; All Rights Reserved.
     ********************************************************************************/
    /*********************************************************************************
     * $Id
     * Description:
     ********************************************************************************/
    require_once('XTemplate/xtpl.php');
    require_once('data/Tracker.php');
    require_once('modules/Threads/Thread.php');
    
    require_once('modules/Administration/Administration.php');
    $admin = new Administration();
    $admin->retrieveSettings("notify");
    
    global $app_strings;
    global $app_list_strings;
    global $mod_strings;
    
    if(!ACLController::checkAccess('Threads', 'edit', true)){
        ACLController::displayNoAccess(false);
        sugar_cleanup(true);
    }
    
    $focus =& new Thread();
    
    if(!isset($_REQUEST['record']))
    $_REQUEST['record'] = "";
    
    if(isset($_REQUEST['record']))
    $focus->retrieve($_REQUEST['record']);
    
    
    //if duplicate record request then clear the Primary key(id) value.
    if(isset($_REQUEST['isDuplicate']) && $_REQUEST['isDuplicate'] == '1') {
        $focus->id = "";
    }
    
    echo "\n<p>\n";
    echo get_module_title($mod_strings['LBL_MODULE_NAME'], $mod_strings['LBL_MODULE_NAME'].": ".$focus->title, true);
    echo "\n</p>\n";
    
    if(!empty($focus->id) && !is_admin($current_user) && $current_user->id != $focus->created_by)
    {
        die('Only administrators or author of a post can edit it');
    }
    
    
    global $theme;
    $theme_path="themes/".$theme."/";
    $image_path=$theme_path."images/";
    
    $xtpl=new XTemplate ('modules/Threads/EditView.html');
    $xtpl->assign("MOD", $mod_strings);
    $xtpl->assign("APP", $app_strings);
    
    if (isset($_REQUEST['return_module']))
    $xtpl->assign("RETURN_MODULE", $_REQUEST['return_module']);
    if (isset($_REQUEST['return_action']))
    $xtpl->assign("RETURN_ACTION", $_REQUEST['return_action']);
    if (!isset($_REQUEST['return_id']))
    die($mod_strings['ERROR_NO_DIRECT_EDIT_ACCESS']);
    // BEGIN SUGARCRM flav=pro ONLY
    else{
        // check that the user has access to the parent forum
        $team_access = get_team_array();
        $tres = $GLOBALS['db']->query("select team_id from forums where id='".$focus->forum_id."'");
        $trow = $GLOBALS['db']->fetchByAssoc($tres);
        if(isset($trow['team_id'])){
            if(!array_key_exists($trow['team_id'], $team_access)){
                die($mod_strings['ERROR_NO_ACCESS_TO_PARENT_FORUM']);
            }
        }
    }
    // END SUGARCRM flav=pro ONLY
    $xtpl->assign("RETURN_ID", $_REQUEST['return_id']);
    
    // setup relationship form values
    if(isset($_REQUEST['account_id'])) $xtpl->assign("ACCOUNT_ID", $_REQUEST['account_id']);
    if(isset($_REQUEST['bug_id'])) $xtpl->assign("BUG_ID", $_REQUEST['bug_id']);
    if(isset($_REQUEST['acase_id'])) $xtpl->assign("ACASE_ID", $_REQUEST['acase_id']);
    if(isset($_REQUEST['opportunity_id'])) $xtpl->assign("OPPORTUNITY_ID", $_REQUEST['opportunity_id']);
    if(isset($_REQUEST['project_id'])) $xtpl->assign("PROJECT_ID", $_REQUEST['project_id']);
    
    if(isset($focus->forum_id))
    $xtpl->assign("FORUM_ID", $focus->forum_id);
    else
    {
        if($_REQUEST['return_module'] != "Forums")
        {
            $xtpl->assign("FORUM_ID", "");
        }
        else
            $xtpl->assign("FORUM_ID", $_REQUEST['return_id']);
    }
    $xtpl->assign("JAVASCRIPT", get_set_focus_js() . get_validate_record_js());
    $xtpl->assign("IMAGE_PATH", $image_path);
    $xtpl->assign("PRINT_URL", "index.php?".$GLOBALS['request_string']);
    
    $xtpl->assign("ID", $focus->id);
    $xtpl->assign("TITLE", $focus->title);
    
    if ($focus->is_sticky == '1')
    $xtpl->assign("IS_STICKY", "checked");
    
    $xtpl->assign("STICKY_VALUE", $focus->is_sticky);
    
    $xtpl->assign('DESCRIPTION_HTML', $focus->description_html);
    $description_html = $focus->description_html;
    ///////////////////////////////////////
    ////	TEXT EDITOR
    if(file_exists('include/SugarTinyMCE.php')) {
        require_once("include/SugarTinyMCE.php");
        $tiny = new SugarTinyMCE();
        $tiny->defaultConfig['cleanup_on_startup']=true;
        $tinyHtml = $tiny->getInstance();
        $xtpl->assign("tiny", $tinyHtml);
        if(!empty($description_html)) {
            $xtpl->assign('HTML_EDITOR', $description_html);
        }
        $xtpl->parse('main.htmlarea');
    }else {
        $xtpl->parse('main.textarea');
    } 
    ////	END TEXT EDITOR
    ///////////////////////////////////////
    
    if(is_admin($current_user))
    $xtpl->parse("main.ShowSticky");
    else
    $xtpl->parse("main.DontShowSticky");
    
    //Add Custom Fields
    require_once('modules/DynamicFields/templates/Files/EditView.php');
    
    $xtpl->assign("THEME", $theme);
    $xtpl->parse("main");
    $xtpl->out("main");
    
    ?>