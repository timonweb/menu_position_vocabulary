<?php
/**
 * @file
 * Provides the vocabulary rule plugin for the Menu Position module.
 */

/**
 * Checks if the current taxonomy term belongs to selected vocabulary.
 *
 * @param $variables array
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 * @return bool
 *   TRUE if condition applies successfully. Otherwise FALSE.
 *
 */
function menu_position_vocabulary_menu_position_condition_vocabulary($variables) {
  if (isset($variables['vids']) && !empty($variables['context']['path'])) {
    $arg = explode('/', $variables['context']['path']);
    // This condition works only for taxonomy pages!
    if ($arg[0] == 'taxonomy'
        && $arg[1] == 'term'
        && is_numeric($arg[2])
        && in_array(taxonomy_term_load($arg[2])->vid, $variables['vids'])
    ) return TRUE;
  }
}

/**
 * Adds form elements for the PLUGIN plugin to the rule configuration form.
 *
 * If this condition was active in the current rule, the plug-in variables will
 * be available in $form_state['#menu-position-rule']['conditions']['PLUGIN'].
 *
 * It is the resposibility of this hook to add any necessary form validation and
 * submission handlers.
 *
 * @param $form array
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param $form_state array
 *   A reference to the current form state.
 */
function menu_position_vocabulary_menu_position_rule_vocabulary_form(&$form, &$form_state) {

  // If this is an existing rule, load the variables stored in the rule.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['vocabulary']) ? $form_state['#menu-position-rule']['conditions']['vocabulary'] : array();

  $form['conditions']['vocabulary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabulary'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'menu_position_vocabulary') . '/menu_position_vocabulary.js'),
    ),
  );

  $vocabularies = taxonomy_vocabulary_get_names();
  $vocabulary_options = array();
  foreach ($vocabularies as $vocabulary) {
    $vocabulary_options[$vocabulary->vid] = $vocabulary->name;
  }
  $form['conditions']['vocabulary']['vids'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#multiple' => TRUE,
    '#default_value' => !empty($variables['vids']) ? $variables['vids'] : '0',
    '#options' => array('0' => t('- None -')) + $vocabulary_options,
  );

  // Add a submit handler.
  $form['#submit'][] = 'menu_position_vocabulary_menu_position_rule_vocabulary_form_submit';
}

/**
 * Prepares the plugin's variables to be stored in the rule.
 *
 * @param $form array
 *   A reference to the "add/edit rule" form array.
 * @param $form_state array
 *   A reference to the current form state, including submitted values.
 */
function menu_position_vocabulary_menu_position_rule_vocabulary_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['vids'])) {
    $form_state['values']['conditions']['vocabulary']['vids'] = $form_state['values']['vids'];
  }
}
