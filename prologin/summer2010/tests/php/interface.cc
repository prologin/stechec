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
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
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
// Le type d'un objet
//
template <>
zval* cxx2lang<zval*, type_objet>(type_objet in)
{
    return cxx2lang<zval*, int>((int)in);
}

template <>
type_objet lang2cxx<zval*, type_objet>(zval* in)
{
    return (type_objet)lang2cxx<zval*, int>(in);
}

///
// Représente une position sur le terrain du jeu.
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
// les caracteristiques d'un objet
//
template <>
zval* cxx2lang<zval*, caracteristiques_objet>(caracteristiques_objet in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.cout));
    add_assoc_zval(ret, "cout", tmp);
    tmp = (cxx2lang<zval*, int>(in.porte));
    add_assoc_zval(ret, "porte", tmp);
    return ret;
}

template <>
caracteristiques_objet lang2cxx<zval*, caracteristiques_objet>(zval* in)
{
    caracteristiques_objet out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "cout", 5, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.cout = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "porte", 6, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.porte = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Représente une unité sur le terrain.
//
template <>
zval* cxx2lang<zval*, unite>(unite in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, position>(in.pos_unite));
    add_assoc_zval(ret, "pos_unite", tmp);
    tmp = (cxx2lang<zval*, int>(in.team));
    add_assoc_zval(ret, "team", tmp);
    tmp = (cxx2lang<zval*, int>(in.ko));
    add_assoc_zval(ret, "ko", tmp);
    tmp = (cxx2lang<zval*, int>(in.pa));
    add_assoc_zval(ret, "pa", tmp);
    tmp = (cxx2lang<zval*, type_objet>(in.objet));
    add_assoc_zval(ret, "objet", tmp);
    tmp = (cxx2lang<zval*, int>(in.id));
    add_assoc_zval(ret, "id", tmp);
    return ret;
}

template <>
unite lang2cxx<zval*, unite>(zval* in)
{
    unite out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "pos_unite", 10, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pos_unite = lang2cxx<zval*, position>(tmp);
    zend_symtable_find(ht, "team", 5, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.team = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "ko", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.ko = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "pa", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pa = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "objet", 6, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.objet = lang2cxx<zval*, type_objet>(tmp);
    zend_symtable_find(ht, "id", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.id = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Représente le type d'une piece.
//
template <>
zval* cxx2lang<zval*, piece>(piece in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.valeur));
    add_assoc_zval(ret, "valeur", tmp);
    tmp = (cxx2lang<zval*, position>(in.pos_piece));
    add_assoc_zval(ret, "pos_piece", tmp);
    tmp = (cxx2lang<zval*, int>(in.tour_apparition));
    add_assoc_zval(ret, "tour_apparition", tmp);
    return ret;
}

template <>
piece lang2cxx<zval*, piece>(zval* in)
{
    piece out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "valeur", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.valeur = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "pos_piece", 10, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pos_piece = lang2cxx<zval*, position>(tmp);
    zend_symtable_find(ht, "tour_apparition", 16, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.tour_apparition = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Renvoie le numero de votre equipe.
//
PHP_FUNCTION(php_api_mon_equipe)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_mon_equipe());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie le score d'une equipe.
//
PHP_FUNCTION(php_api_score)
{
    zval* _team;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_team) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, int>(api_score(lang2cxx<zval*, int>(_team)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie le nombre d'equipes sur la map
//
PHP_FUNCTION(php_api_nombre_equipes)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_nombre_equipes());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie le numéro du tour actuel.
//
PHP_FUNCTION(php_api_tour_actuel)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_tour_actuel());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie les pieces qui sont sur la map
//
PHP_FUNCTION(php_api_pieces_en_jeu)
{
        try {
zval* ret = cxx2lang_array(api_pieces_en_jeu());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie le 10 prochaines pieces a vennir
//
PHP_FUNCTION(php_api_pieces_a_vennir)
{
        try {
zval* ret = cxx2lang_array(api_pieces_a_vennir());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne la liste des unités actuellement en jeu.
//
PHP_FUNCTION(php_api_unites)
{
        try {
zval* ret = cxx2lang_array(api_unites());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne les caracteristiques de l'objet.
//
PHP_FUNCTION(php_api_proprietes_objet)
{
    zval* _to;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_to) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, caracteristiques_objet>(api_proprietes_objet(lang2cxx<zval*, type_objet>(_to)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Déplace une unité vers une position à portée.
//
PHP_FUNCTION(php_api_deplacer)
{
    zval* _cible;
    zval* _pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_cible, &_pos) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_deplacer(lang2cxx<zval*, position>(_cible), lang2cxx<zval*, position>(_pos)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Achete un objet
//
PHP_FUNCTION(php_api_acheter_objet)
{
    zval* _cible;
    zval* _objet;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_cible, &_objet) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_acheter_objet(lang2cxx<zval*, position>(_cible), lang2cxx<zval*, type_objet>(_objet)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// utilise un objet
//
PHP_FUNCTION(php_api_utiliser)
{
    zval* _attaquant;
    zval* _cible;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_attaquant, &_cible) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_utiliser(lang2cxx<zval*, position>(_attaquant), lang2cxx<zval*, position>(_cible)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
PHP_FUNCTION(php_api_achever_la_partie)
{
        try {
zval* ret = cxx2lang<zval*, erreur>(api_achever_la_partie());
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
// Affiche le contenu d'une valeur de type type_objet
//
PHP_FUNCTION(php_api_afficher_type_objet)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_type_objet(lang2cxx<zval*, type_objet>(_v));
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
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
PHP_FUNCTION(php_api_afficher_caracteristiques_objet)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_caracteristiques_objet(lang2cxx<zval*, caracteristiques_objet>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type unite
//
PHP_FUNCTION(php_api_afficher_unite)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_unite(lang2cxx<zval*, unite>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type piece
//
PHP_FUNCTION(php_api_afficher_piece)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_piece(lang2cxx<zval*, piece>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

static function_entry module_functions_table[] = {
    PHP_FALIAS(mon_equipe, php_api_mon_equipe, NULL)    PHP_FALIAS(score, php_api_score, NULL)    PHP_FALIAS(nombre_equipes, php_api_nombre_equipes, NULL)    PHP_FALIAS(tour_actuel, php_api_tour_actuel, NULL)    PHP_FALIAS(pieces_en_jeu, php_api_pieces_en_jeu, NULL)    PHP_FALIAS(pieces_a_vennir, php_api_pieces_a_vennir, NULL)    PHP_FALIAS(unites, php_api_unites, NULL)    PHP_FALIAS(proprietes_objet, php_api_proprietes_objet, NULL)    PHP_FALIAS(deplacer, php_api_deplacer, NULL)    PHP_FALIAS(acheter_objet, php_api_acheter_objet, NULL)    PHP_FALIAS(utiliser, php_api_utiliser, NULL)    PHP_FALIAS(achever_la_partie, php_api_achever_la_partie, NULL)    PHP_FALIAS(afficher_erreur, php_api_afficher_erreur, NULL)    PHP_FALIAS(afficher_type_objet, php_api_afficher_type_objet, NULL)    PHP_FALIAS(afficher_position, php_api_afficher_position, NULL)    PHP_FALIAS(afficher_caracteristiques_objet, php_api_afficher_caracteristiques_objet, NULL)    PHP_FALIAS(afficher_unite, php_api_afficher_unite, NULL)    PHP_FALIAS(afficher_piece, php_api_afficher_piece, NULL)    {NULL, NULL, NULL}
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
// Fonction appellée au début de la partie.
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
// Fonction appellée pour la phase de jeu.
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
// Fonction appellée à la fin de la partie.
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
// Affiche le contenu d'une valeur de type type_objet
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//

///
// Affiche le contenu d'une valeur de type unite
//

///
// Affiche le contenu d'une valeur de type piece
//

