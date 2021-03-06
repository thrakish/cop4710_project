<?php
// place to put various helper functions

// logs the variable out to the webpage in a pretty printed fashion
// log was taken. 

function console_log($var) {
  print "<pre>";
  var_dump($var);
  print "</pre>";
}
                                                                      
// encrypts a plain-text password into a hash before inserting into database
function encrypt_password($pw) {

  $hash = password_hash($pw, PASSWORD_BCRYPT , array('cost' => 12));

  return $hash;

}

// checks a hashed password against a plain-text one.
function check_password($pw, $hash) {

  return password_verify($pw, $hash);

}

// checks the key of an array and returns the default if it is not set or empty.
function check_field ($params = array(), $key, $default = NULL) {

  return isset($params[$key]) && !empty($params[$key])? $params[$key]: $default;

}

// checks for main.jpg in the entities directory
function entity_has_pic ($type, $id) {

  global $configs;

  $dir_root = $configs['dir_root'];
  $site_root = $configs['site_root'];

  $entity_pic_uri = $dir_root . "/res/$type/$id/main.jpg";

  // see if the file exists
  if (file_exists($entity_pic_uri)) {

    return TRUE;

  }

  return FALSE;

}

// keeping this simple. only allowing 1 main picture.
function create_entity_pic ($type, $id, $file) {

  global $configs;

  $dir_root = $configs['dir_root'];

  $entity_pic_uri = $dir_root . "/res/$type/$id";

  // see if the dir exists
  if (!is_dir($entity_pic_uri)) {

    // if not create dir
    mkdir($entity_pic_uri);
    
  }

  // delete the file if it exists
  if (entity_has_pic($type, $id)) {

    destroy_entity_pic($type, $id);

  }

  // add file to dir
  return move_uploaded_file($file, $entity_pic_uri . "/main.jpg");

}

function get_entity_pic_uri ($type, $id, $default = "default.jpg") {

  global $configs;

  $site_root = $configs['site_root'];

  return $site_root.'/res/'.$type.'/'.$id.'/main.jpg';

}

function get_entity_pic_path ($type, $id, $default = "default.jpg") {

  global $configs;

  $dir_root = $configs['dir_root'];

  return $dir_root.'/res/'.$type.'/'.$id.'/main.jpg';

}

// creates an image tag for templates if the entity has profile pic
function load_entity_pic ($type, $id, $style = "thumbnail", $default = "default.jpg") {

  global $configs;

  $site_root = $configs['site_root'];

  if (entity_has_pic($type, $id)) {

    // if it exists return img tag with style and link
    return '<div class="entity '.$type.' '.$style.'"><img src="'.get_entity_pic_uri($type, $id).'"></img></div>';
    
  }

  // if it doesn't exist return default pic.
  return '<div class="entity '.$type.' '.$style.'" ><img src="'.$site_root.'/res/'.$type.'/'.$default.'"></img></div>';

}

function destroy_entity_pic ($type, $id) {

  global $configs;

  $dir_root = $configs['dir_root'];

  $entity_pic_uri = $dir_root . "/res/$type/$id/main.jpg";

  if (file_exists($entity_pic_uri)) {

    unlink($entity_pic_uri);

    return TRUE;
    
  }

  return FALSE;

}

// this function simply returns the string of the path
// to the entity partial from the classes directory.
function get_entity_partial ($entity, $partial) {

  global $configs;

  $entity_partial_dir = $configs['entity_partial_dir'];

  return "$entity_partial_dir/$entity/$partial.inc";

}

function get_mail_partial ($partial) {

  return "partials/mail/$partial.inc";

}

/*
 * excluded for now. will include if time allows.
function send_auth_email ($username, $email, $auth_url) {

  include get_mail_partial("auth");

  mail($email, "Welcome to website", $template);

}
*/

function go_home () {

  global $configs;  

  $site_root = $configs['site_root'];

  header("location: $site_root");

}

// goes back to the refering page. useful for errors arising from
// submitting a form (e.g. submitting a new user. if a PDO exception
// is thrown, this function is called sending the user back to
// the form with an error message)
function go_back () {

  if (!isset($_SERVER['HTTP_REFERER'])) {

    go_home;

  }

  $referer = $_SERVER['HTTP_REFERER'];

  header("location: $referer");

}

// all entities with a 'view' action will usually follow the
// url pattern of 'resource/[id]', this just performs a redirect
// to the entity type with the following id.
function redirect_entity_view($resource_name, $id) {

  global $configs;

  $site_root = $configs['site_root'];

  // redirect to user/12, university/3, etc.
  header("location: $site_root/$resource_name/$id");

}

// User message. Appears at the top of the screen after a redirect.
// is set using the session global and is cleared after it is retrieved.
// the 'type' is used for styling purposes.
function set_message ($message, $message_type = "info") {

  // return false if there is no session
  if (!isset($_SESSION)) {
    return FALSE;
  }

  // set the message and type to be used on the next page.
  $_SESSION['message'] = $message;

  $_SESSION['message_type'] = $message_type;


}

// clears message so the message doesn't persist over multiple page requests.
function clear_message () {
  unset($_SESSION['message']);
  unset($_SESSION['message_type']);
}

function get_message_html () {

  if (!isset($_SESSION['message'])) {
    return "";
  }

  $message = $_SESSION["message"];
  $message_type = $_SESSION["message_type"];

  include "partials/message.inc";

  // clear message info from the session;
  clear_message();

  // return template defined in the include file.
  return $template;

}

// redirects to a path relative to the site root.
// you could user "login" for example and the "http://cop4710_project/" part is included.
function redirect_to_url ($url) {

  global $configs;

  $site_root = $configs['site_root'];

  header("location: $site_root/$url");

}

function get_pagination_from_id ($id, $inc) {
  // calculates the start and end on an id.
  $start = (int)((int)$id / $inc) * $inc;
  $end = ($start / $inc + 1) * $inc;

  return array($start, $end);

}


/*
  _____             _ _   _          _   _             
 / ____|           (_) | (_)        | | (_)            
| (___   __ _ _ __  _| |_ _ ______ _| |_ _  ___  _ __  
 \___ \ / _` | '_ \| | __| |_  / _` | __| |/ _ \| '_ \ 
 ____) | (_| | | | | | |_| |/ / (_| | |_| | (_) | | | |
|_____/ \__,_|_| |_|_|\__|_/___\__,_|\__|_|\___/|_| |_|
*/
/* verification and e-mail is being deferred unless we
 * have time in this project to do it.
function sanitize_email_input (&$email, &$username) {

  $email = sanitize_xss($email);

  $username = sanitize_xss($username);

  $email = str_replace(array('\n', '\r'), '', $email);

  $username = str_replace(array('\n', '\r'), '', $username);

}
*/
// strip out html tags from user input.
function sanitize_xss ($input) {

  return strip_tags($input);  

}

?>
