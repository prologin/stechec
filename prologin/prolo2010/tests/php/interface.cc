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
// Le type d'une unité sur le terrain
//
template <>
zval* cxx2lang<zval*, type_unite>(type_unite in)
{
    return cxx2lang<zval*, int>((int)in);
}

template <>
type_unite lang2cxx<zval*, type_unite>(zval* in)
{
    return (type_unite)lang2cxx<zval*, int>(in);
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
// Contient les informations sur la taille du terrain du jeu.
//
template <>
zval* cxx2lang<zval*, taille_terrain>(taille_terrain in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.taille));
    add_assoc_zval(ret, "taille", tmp);
    tmp = (cxx2lang<zval*, int>(in.min_coord));
    add_assoc_zval(ret, "min_coord", tmp);
    tmp = (cxx2lang<zval*, int>(in.max_coord));
    add_assoc_zval(ret, "max_coord", tmp);
    return ret;
}

template <>
taille_terrain lang2cxx<zval*, taille_terrain>(zval* in)
{
    taille_terrain out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "taille", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.taille = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "min_coord", 10, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.min_coord = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "max_coord", 10, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.max_coord = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
zval* cxx2lang<zval*, caracs>(caracs in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.pa_init));
    add_assoc_zval(ret, "pa_init", tmp);
    tmp = (cxx2lang<zval*, int>(in.portee));
    add_assoc_zval(ret, "portee", tmp);
    return ret;
}

template <>
caracs lang2cxx<zval*, caracs>(zval* in)
{
    caracs out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "pa_init", 8, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pa_init = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "portee", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.portee = lang2cxx<zval*, int>(tmp);
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
    tmp = (cxx2lang<zval*, position>(in.pos));
    add_assoc_zval(ret, "pos", tmp);
    tmp = (cxx2lang<zval*, bool>(in.ennemi));
    add_assoc_zval(ret, "ennemi", tmp);
    tmp = (cxx2lang<zval*, type_unite>(in.type_unite_actuel));
    add_assoc_zval(ret, "type_unite_actuel", tmp);
    tmp = (cxx2lang<zval*, type_unite>(in.vrai_type_unite));
    add_assoc_zval(ret, "vrai_type_unite", tmp);
    tmp = (cxx2lang<zval*, int>(in.ko));
    add_assoc_zval(ret, "ko", tmp);
    tmp = (cxx2lang<zval*, int>(in.pa));
    add_assoc_zval(ret, "pa", tmp);
    tmp = (cxx2lang<zval*, int>(in.attaques));
    add_assoc_zval(ret, "attaques", tmp);
    tmp = (cxx2lang<zval*, int>(in.attaques_gratuites));
    add_assoc_zval(ret, "attaques_gratuites", tmp);
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
    zend_symtable_find(ht, "pos", 4, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pos = lang2cxx<zval*, position>(tmp);
    zend_symtable_find(ht, "ennemi", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.ennemi = lang2cxx<zval*, bool>(tmp);
    zend_symtable_find(ht, "type_unite_actuel", 18, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.type_unite_actuel = lang2cxx<zval*, type_unite>(tmp);
    zend_symtable_find(ht, "vrai_type_unite", 16, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.vrai_type_unite = lang2cxx<zval*, type_unite>(tmp);
    zend_symtable_find(ht, "ko", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.ko = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "pa", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pa = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "attaques", 9, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.attaques = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "attaques_gratuites", 19, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.attaques_gratuites = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "id", 3, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.id = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
zval* cxx2lang<zval*, cartes>(cartes in)
{
    zval* ret;
    zval* tmp;
    MAKE_STD_ZVAL(ret);
    array_init(ret);
    tmp = (cxx2lang<zval*, int>(in.potion));
    add_assoc_zval(ret, "potion", tmp);
    tmp = (cxx2lang<zval*, int>(in.deguisement));
    add_assoc_zval(ret, "deguisement", tmp);
    tmp = (cxx2lang<zval*, int>(in.banzai));
    add_assoc_zval(ret, "banzai", tmp);
    tmp = (cxx2lang<zval*, int>(in.pacifisme));
    add_assoc_zval(ret, "pacifisme", tmp);
    return ret;
}

template <>
cartes lang2cxx<zval*, cartes>(zval* in)
{
    cartes out;
    if (in->type != IS_ARRAY) {
        zend_error(E_WARNING, "parameter is not a structure");
        throw 42;
    }
    zval* tmp;
    HashTable* ht = Z_ARRVAL_P(in);
    zend_symtable_find(ht, "potion", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.potion = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "deguisement", 12, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.deguisement = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "banzai", 7, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.banzai = lang2cxx<zval*, int>(tmp);
    zend_symtable_find(ht, "pacifisme", 10, (void**)&tmp);
    tmp = (zval*)tmp->value.ht;
    out.pacifisme = lang2cxx<zval*, int>(tmp);
    return out;
}

///
// Renvoie le nombre de points de commandements.
//
PHP_FUNCTION(php_api_nombre_pc)
{
        try {
zval* ret = cxx2lang<zval*, int>(api_nombre_pc());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie le nombre d'unités en jeu.
//
PHP_FUNCTION(php_api_nombre_unites)
{
    zval* _ennemi;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_ennemi) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, int>(api_nombre_unites(lang2cxx<zval*, bool>(_ennemi)));
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
// Renvoie la position du spawn (ennemi ou non).
//
PHP_FUNCTION(php_api_pos_renfort)
{
    zval* _ennemi;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_ennemi) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, position>(api_pos_renfort(lang2cxx<zval*, bool>(_ennemi)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
PHP_FUNCTION(php_api_caracteristiques)
{
    zval* _tu;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_tu) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, caracs>(api_caracteristiques(lang2cxx<zval*, type_unite>(_tu)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
PHP_FUNCTION(php_api_mes_cartes)
{
        try {
zval* ret = cxx2lang<zval*, cartes>(api_mes_cartes());
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
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
PHP_FUNCTION(php_api_taille_terrain_actuelle)
{
        try {
zval* ret = cxx2lang<zval*, taille_terrain>(api_taille_terrain_actuelle());
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
PHP_FUNCTION(php_api_potion)
{
    zval* _cible;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_cible) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_potion(lang2cxx<zval*, position>(_cible)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
PHP_FUNCTION(php_api_deguisement)
{
    zval* _cible;
    zval* _nouveau_type;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_cible, &_nouveau_type) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_deguisement(lang2cxx<zval*, position>(_cible), lang2cxx<zval*, type_unite>(_nouveau_type)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
PHP_FUNCTION(php_api_banzai)
{
    zval* _cible;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_cible) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_banzai(lang2cxx<zval*, position>(_cible)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
PHP_FUNCTION(php_api_pacifisme)
{
        try {
zval* ret = cxx2lang<zval*, erreur>(api_pacifisme());
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
// Relève une unité n'ayant plus de marqueurs de KO.
//
PHP_FUNCTION(php_api_relever)
{
    zval* _cible;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_cible) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_relever(lang2cxx<zval*, position>(_cible)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Attaque une autre unité.
//
PHP_FUNCTION(php_api_attaquer)
{
    zval* _attaquant;
    zval* _cible;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &_attaquant, &_cible) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_attaquer(lang2cxx<zval*, position>(_attaquant), lang2cxx<zval*, position>(_cible)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Fait apparaitre une unité sur la case de spawn.
//
PHP_FUNCTION(php_api_renfort)
{
    zval* _quoi;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_quoi) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
zval* ret = cxx2lang<zval*, erreur>(api_renfort(lang2cxx<zval*, type_unite>(_quoi)));
    RETURN_ZVAL(ret, 0, 0);
    } catch (...) { RETURN_NULL(); }
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
PHP_FUNCTION(php_api_annuler)
{
        try {
zval* ret = cxx2lang<zval*, bool>(api_annuler());
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
// Affiche le contenu d'une valeur de type type_unite
//
PHP_FUNCTION(php_api_afficher_type_unite)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_type_unite(lang2cxx<zval*, type_unite>(_v));
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
// Affiche le contenu d'une valeur de type taille_terrain
//
PHP_FUNCTION(php_api_afficher_taille_terrain)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_taille_terrain(lang2cxx<zval*, taille_terrain>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

///
// Affiche le contenu d'une valeur de type caracs
//
PHP_FUNCTION(php_api_afficher_caracs)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_caracs(lang2cxx<zval*, caracs>(_v));
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
// Affiche le contenu d'une valeur de type cartes
//
PHP_FUNCTION(php_api_afficher_cartes)
{
    zval* _v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &_v) == FAILURE)
    {
        RETURN_NULL();
    }

        try {
api_afficher_cartes(lang2cxx<zval*, cartes>(_v));
    RETURN_NULL();
    } catch (...) { RETURN_NULL(); }
}

static function_entry module_functions_table[] = {
    PHP_FALIAS(nombre_pc, php_api_nombre_pc, NULL)
    PHP_FALIAS(nombre_unites, php_api_nombre_unites, NULL)
    PHP_FALIAS(tour_actuel, php_api_tour_actuel, NULL)
    PHP_FALIAS(pos_renfort, php_api_pos_renfort, NULL)
    PHP_FALIAS(caracteristiques, php_api_caracteristiques, NULL)
    PHP_FALIAS(mes_cartes, php_api_mes_cartes, NULL)
    PHP_FALIAS(unites, php_api_unites, NULL)
    PHP_FALIAS(taille_terrain_actuelle, php_api_taille_terrain_actuelle, NULL)
    PHP_FALIAS(potion, php_api_potion, NULL)
    PHP_FALIAS(deguisement, php_api_deguisement, NULL)
    PHP_FALIAS(banzai, php_api_banzai, NULL)
    PHP_FALIAS(pacifisme, php_api_pacifisme, NULL)
    PHP_FALIAS(deplacer, php_api_deplacer, NULL)
    PHP_FALIAS(relever, php_api_relever, NULL)
    PHP_FALIAS(attaquer, php_api_attaquer, NULL)
    PHP_FALIAS(renfort, php_api_renfort, NULL)
    PHP_FALIAS(annuler, php_api_annuler, NULL)
    PHP_FALIAS(afficher_erreur, php_api_afficher_erreur, NULL)
    PHP_FALIAS(afficher_type_unite, php_api_afficher_type_unite, NULL)
    PHP_FALIAS(afficher_position, php_api_afficher_position, NULL)
    PHP_FALIAS(afficher_taille_terrain, php_api_afficher_taille_terrain, NULL)
    PHP_FALIAS(afficher_caracs, php_api_afficher_caracs, NULL)
    PHP_FALIAS(afficher_unite, php_api_afficher_unite, NULL)
    PHP_FALIAS(afficher_cartes, php_api_afficher_cartes, NULL)
    {NULL, NULL, NULL}
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
// Fonction appellée pour la phase de retrait de KO.
//
extern "C" position retirer_ko()
{
    zval* ret;
    zval* fname;
    _init_php();

    MAKE_STD_ZVAL(ret);
    MAKE_STD_ZVAL(fname);
    ZVAL_STRING(fname, "retirer_ko", 1);
    if (call_user_function(EG(function_table), NULL, fname, ret, 0, NULL TSRMLS_CC) == FAILURE)
    {
        abort();
    }
    position cxxret = lang2cxx<zval*, position>(ret);
    zval_ptr_dtor(&ret);
    zval_ptr_dtor(&fname);
    return cxxret;
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

