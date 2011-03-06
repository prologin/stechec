///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_php.rb
//

#include "interface.hh"

static void _init_php();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
zval* cxx2lang<zval*, int>(int in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    ZVAL_LONG(x, (long)in);
    return x;
}

template <>
zval* cxx2lang<zval*, bool>(bool in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    ZVAL_BOOL(x, in);
    return x;
}

template <>
zval* cxx2lang<zval*, std::string>(std::string in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    ZVAL_STRINGL(x, in.c_str(), in.length(), true);
    return x;
}

template <typename Cxx>
zval* cxx2lang_array(const std::vector<Cxx>& in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    array_init(x);

    size_t s = in.size();

    for (size_t i = 0; i < s; ++i)
      add_next_index_zval(x, cxx2lang<zval*, Cxx>(in[i]));

    return x;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<zval*, int>(zval* in)
{
    return (int)Z_LVAL_P(in);
}

template <>
bool lang2cxx<zval*, bool>(zval* in)
{
    return Z_BVAL_P(in);
}

template <>
std::string lang2cxx<zval*, std::string>(zval* in)
{
    return in->value.str.val;
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(zval* in)
{
    HashTable* ht = Z_ARRVAL_P(in);
    std::vector<Cxx> out;
    size_t s = zend_hash_num_elements(ht);

    for (size_t i = 0; i < s; ++i)
    {
        zval* v;
        zend_hash_index_find(ht, i, (void**)&v);
        out.push_back(lang2cxx<zval*, Cxx>(v));
    }

    return out;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action
//
template <>
zval* cxx2lang<zval*, erreur>(erreur in)
{
    return cxx2lang<zval*, int>((int)in);
}

template <>
erreur lang2cxx<zval*, erreur>(zval* in)
{
    return (erreur)lang2cxx<zval*, int>(in);
}

///
// Énumération représentant les différents types de case
//
template <>
zval* cxx2lang<zval*, type_case>(type_case in)
{
    return cxx2lang<zval*, int>((int)in);
}

template <>
type_case lang2cxx<zval*, type_case>(zval* in)
{
    return (type_case)lang2cxx<zval*, int>(in);
}

///
// Énumération représentant les différents types de bonii
//
template <>
zval* cxx2lang<zval*, type_bonus>(type_bonus in)
{
    return cxx2lang<zval*, int>((int)in);
}

template <>
type_bonus lang2cxx<zval*, type_bonus>(zval* in)
{
    return (type_bonus)lang2cxx<zval*, int>(in);
}

///
// Représente une position sur le terrain du jeu
//
template <>
zval* cxx2lang<zval*, position>(position in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.x));
    add_assoc_zval(ret, "x", tmp);
    tmp = (cxx2lang<zval*, int>(in.y));
    add_assoc_zval(ret, "y", tmp);
    return ret;
}

template <>
position lang2cxx<zval*, position>(zval* in)
{
    position out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "x", 2, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.x = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "y", 2, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.y = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Caracteristiques d'une source d'énergie
//
template <>
zval* cxx2lang<zval*, source_energie>(source_energie in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.id));
    add_assoc_zval(ret, "id", tmp);
    tmp = (cxx2lang<zval*, position>(in.pos));
    add_assoc_zval(ret, "pos", tmp);
    tmp = (cxx2lang<zval*, int>(in.coef));
    add_assoc_zval(ret, "coef", tmp);
    return ret;
}

template <>
source_energie lang2cxx<zval*, source_energie>(zval* in)
{
    source_energie out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "id", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.id = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "pos", 4, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pos = lang2cxx<zval*, position>(tmp);
    zend_symtable_find(ht, "coef", 5, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.coef = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Représente une traînée de moto sur le terrain
//
template <>
zval* cxx2lang<zval*, trainee_moto>(trainee_moto in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.id));
    add_assoc_zval(ret, "id", tmp);
    tmp = cxx2lang_array(in.emplacement);
    add_assoc_zval(ret, "emplacement", tmp);
    tmp = (cxx2lang<zval*, int>(in.team));
    add_assoc_zval(ret, "team", tmp);
    return ret;
}

template <>
trainee_moto lang2cxx<zval*, trainee_moto>(zval* in)
{
    trainee_moto out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "id", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.id = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "emplacement", 12, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.emplacement = lang2cxx_array<position>(tmp);
    zend_symtable_find(ht, "team", 5, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.team = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Retourne le numéro de votre équipe
//
PHP_FUNCTION(php_api_mon_equipe)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_mon_equipe());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne les scores de chaque équipe
//
PHP_FUNCTION(php_api_scores)
{
        try {
zval* ret = cxx2lang_array(api_scores());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne le nombre d'équipes sur le terrain
//
PHP_FUNCTION(php_api_nombre_equipes)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_nombre_equipes());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne le numéro du tour actuel
//
PHP_FUNCTION(php_api_tour_actuel)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_tour_actuel());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne la liste des sources d'énergie
//
PHP_FUNCTION(php_api_sources_energie)
{
        try {
zval* ret = cxx2lang_array(api_sources_energie());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne la liste des traînées de moto
//
PHP_FUNCTION(php_api_trainees_moto)
{
        try {
zval* ret = cxx2lang_array(api_trainees_moto());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne le type d'une case
//
PHP_FUNCTION(php_api_regarder_type_case)
{
    zval* _pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_pos) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, type_case>(api_regarder_type_case(lang2cxx<zval*, position>(_pos)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne le type de bonus d'une case
//
PHP_FUNCTION(php_api_regarder_type_bonus)
{
    zval* _pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_pos) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, type_bonus>(api_regarder_type_bonus(lang2cxx<zval*, position>(_pos)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne la liste des bonus d'une équipe
//
PHP_FUNCTION(php_api_regarder_bonus)
{
    zval* _equipe;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_equipe) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang_array(api_regarder_bonus(lang2cxx<zval*, int>(_equipe)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Déplace une moto
//
PHP_FUNCTION(php_api_deplacer)
{
    zval* _id;
    zval* _de;
    zval* _vers;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &_id, &_de, &_vers) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_deplacer(lang2cxx<zval*, int>(_id), lang2cxx<zval*, position>(_de), lang2cxx<zval*, position>(_vers)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
PHP_FUNCTION(php_api_couper_trainee_moto)
{
    zval* _id;
    zval* _entre;
    zval* _et;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &_id, &_entre, &_et) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_couper_trainee_moto(lang2cxx<zval*, int>(_id), lang2cxx<zval*, position>(_entre), lang2cxx<zval*, position>(_et)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Annuler l'action précédente
//
PHP_FUNCTION(php_api_cancel)
{
        try {
zval* ret = cxx2lang<zval*, erreur>(api_cancel());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Enrouler la traînée de moto en un point
//
PHP_FUNCTION(php_api_enrouler)
{
    zval* _id;
    zval* _point;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_id, &_point) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_enrouler(lang2cxx<zval*, int>(_id), lang2cxx<zval*, position>(_point)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Régénère une source d'énergie à son maximal
//
PHP_FUNCTION(php_api_regenerer_source_energie)
{
    zval* _id;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_id) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_regenerer_source_energie(lang2cxx<zval*, int>(_id)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Allonge le tour en rajoutant des points d'action
//
PHP_FUNCTION(php_api_allonger_pa)
{
        try {
zval* ret = cxx2lang<zval*, erreur>(api_allonger_pa());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
PHP_FUNCTION(php_api_agrandir_trainee_moto)
{
    zval* _id;
    zval* _longueur;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_id, &_longueur) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_agrandir_trainee_moto(lang2cxx<zval*, int>(_id), lang2cxx<zval*, int>(_longueur)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
PHP_FUNCTION(php_api_poser_point_croisement)
{
    zval* _point;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_point) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_poser_point_croisement(lang2cxx<zval*, position>(_point)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
PHP_FUNCTION(php_api_fusionner)
{
    zval* _id1;
    zval* _pos1;
    zval* _id2;
    zval* _pos2;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &_id1, &_pos1, &_id2, &_pos2) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_fusionner(lang2cxx<zval*, int>(_id1), lang2cxx<zval*, position>(_pos1), lang2cxx<zval*, int>(_id2), lang2cxx<zval*, position>(_pos2)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type erreur
//
PHP_FUNCTION(php_api_afficher_erreur)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_erreur(lang2cxx<zval*, erreur>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type type_case
//
PHP_FUNCTION(php_api_afficher_type_case)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_type_case(lang2cxx<zval*, type_case>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type type_bonus
//
PHP_FUNCTION(php_api_afficher_type_bonus)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_type_bonus(lang2cxx<zval*, type_bonus>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type position
//
PHP_FUNCTION(php_api_afficher_position)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_position(lang2cxx<zval*, position>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type source_energie
//
PHP_FUNCTION(php_api_afficher_source_energie)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_source_energie(lang2cxx<zval*, source_energie>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
PHP_FUNCTION(php_api_afficher_trainee_moto)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_trainee_moto(lang2cxx<zval*, trainee_moto>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

static function_entry module_functions_table[] = {
    PHP_FALIAS(mon_equipe, php_api_mon_equipe, NULL)    PHP_FALIAS(scores, php_api_scores, NULL)    PHP_FALIAS(nombre_equipes, php_api_nombre_equipes, NULL)    PHP_FALIAS(tour_actuel, php_api_tour_actuel, NULL)    PHP_FALIAS(sources_energie, php_api_sources_energie, NULL)    PHP_FALIAS(trainees_moto, php_api_trainees_moto, NULL)    PHP_FALIAS(regarder_type_case, php_api_regarder_type_case, NULL)    PHP_FALIAS(regarder_type_bonus, php_api_regarder_type_bonus, NULL)    PHP_FALIAS(regarder_bonus, php_api_regarder_bonus, NULL)    PHP_FALIAS(deplacer, php_api_deplacer, NULL)    PHP_FALIAS(couper_trainee_moto, php_api_couper_trainee_moto, NULL)    PHP_FALIAS(cancel, php_api_cancel, NULL)    PHP_FALIAS(enrouler, php_api_enrouler, NULL)    PHP_FALIAS(regenerer_source_energie, php_api_regenerer_source_energie, NULL)    PHP_FALIAS(allonger_pa, php_api_allonger_pa, NULL)    PHP_FALIAS(agrandir_trainee_moto, php_api_agrandir_trainee_moto, NULL)    PHP_FALIAS(poser_point_croisement, php_api_poser_point_croisement, NULL)    PHP_FALIAS(fusionner, php_api_fusionner, NULL)    PHP_FALIAS(afficher_erreur, php_api_afficher_erreur, NULL)    PHP_FALIAS(afficher_type_case, php_api_afficher_type_case, NULL)    PHP_FALIAS(afficher_type_bonus, php_api_afficher_type_bonus, NULL)    PHP_FALIAS(afficher_position, php_api_afficher_position, NULL)    PHP_FALIAS(afficher_source_energie, php_api_afficher_source_energie, NULL)    PHP_FALIAS(afficher_trainee_moto, php_api_afficher_trainee_moto, NULL)    {NULL, NULL, NULL}
};
static zend_module_entry api_module_entry = {
    STANDARD_MODULE_HEADER,
    "api",
    module_functions_table,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "1.0",
    STANDARD_MODULE_PROPERTIES
};

static void _init_php()
{
    static int initialized = 0;
    if (initialized)
        return;
    initialized = 1;

    char* argv[] = { "champion", NULL };
    char buffer[1024];
    const char* path;
    zend_file_handle script;

    path = getenv("CHAMPION_PATH");
    if (!path)
        path = ".";

    snprintf(buffer, 1024, "include('%s/%s.php');", path, "prologin");

    php_embed_init(1, argv PTSRMLS_CC);
    zend_startup_module(&api_module_entry);
    zend_eval_string(buffer, NULL, "PHP to Stechec interface");
}

///
// Fonction appellée au début de la partie
//
extern "C" void init_game()
{
    zval* ret;
    zval* fname;
    _init_php();

    MAKE_STD_ZVAL(ret);
    MAKE_STD_ZVAL(fname);
    ZVAL_STRING(fname, "init_game", 1);
    if (call_user_function(EG(function_table), NULL, fname, ret, 0, NULL TSRMLS_CC) == FAILURE)
    {
        abort();
    }
    zval_ptr_dtor(&ret);
    zval_ptr_dtor(&fname);
}

///
// Fonction appellée pour la phase de jeu
//
extern "C" void jouer()
{
    zval* ret;
    zval* fname;
    _init_php();

    MAKE_STD_ZVAL(ret);
    MAKE_STD_ZVAL(fname);
    ZVAL_STRING(fname, "jouer", 1);
    if (call_user_function(EG(function_table), NULL, fname, ret, 0, NULL TSRMLS_CC) == FAILURE)
    {
        abort();
    }
    zval_ptr_dtor(&ret);
    zval_ptr_dtor(&fname);
}

///
// Fonction appellée à la fin de la partie
//
extern "C" void end_game()
{
    zval* ret;
    zval* fname;
    _init_php();

    MAKE_STD_ZVAL(ret);
    MAKE_STD_ZVAL(fname);
    ZVAL_STRING(fname, "end_game", 1);
    if (call_user_function(EG(function_table), NULL, fname, ret, 0, NULL TSRMLS_CC) == FAILURE)
    {
        abort();
    }
    zval_ptr_dtor(&ret);
    zval_ptr_dtor(&fname);
}

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type type_bonus
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type source_energie
//

///
// Affiche le contenu d'une valeur de type trainee_moto
//

