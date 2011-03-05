# -*- coding: iso-8859-1 -*-
from _api import *

# Taille de départ du terrain.
TAILLE_DEPART = 19

# Le nombre maximal d'unités pouvant appartenir à une équipe.
NBR_MAX_UNITES = 10

# Le temps, en nombre de tours, entre deux rétrécissement du terrain.
TEMPS_RETRECISSEMENT = 5

# Le nombre maximum en jeu de chaque carte.
MAX_CARTES = 2

# Énumération représentant une erreur renvoyée par une des fonctions d'action.
(
    OK, # <- aucune erreur n'est survenue
    POSITION_INVALIDE, # <- la position spécifiée est invalide
    CASE_OCCUPEE, # <- la case sur laquelle vous tentez de vous déplacer est occupée
    PAS_A_PORTEE, # <- la cible n'est pas à portée
    UNITE_KO, # <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
    UNITE_DEBOUT, # <- l'unité que vous essayez de relever est déjà debout
    QUOTA_DEPASSE, # <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé
    PLUS_DE_PA, # <- cette unité a réalisé toutes ses actions
    DEJA_ATTAQUE, # <- cette unité a déjà attaqué
    UNITE_INTERDITE, # <- cette unité ne peut pas être amenée en renfort
    RENFORT_IMPOSSIBLE, # <- une unité est déjà présente sur le spawn
    PAS_A_MOI, # <- l'unité ciblée n'est pas à vous
    PLUS_DE_CARTES, # <- il n'y a plus de cartes du type spécifié dans votre main
    PHASE_CARTES_TERMINEE, # <- vous ne pouvez plus poser de cartes car vous avez fait une action
) = range(14)


# Le type d'une unité sur le terrain
(
    PERROQUET, # <- 1 PA et portée de 1
    SINGE, # <- 2 PA et portée de 3
    CHAT, # <- 5 PA et portée de 1
    KANGOUROU, # <- 3 PA et portée de 2 (explose !)
) = range(4)


try:
    from collections import namedtuple
except ImportError:
    from operator import itemgetter as _itemgetter
    from keyword import iskeyword as _iskeyword
    import sys as _sys

    def namedtuple(typename, field_names, verbose=False):
        """Returns a new subclass of tuple with named fields.

        >>> Point = namedtuple('Point', 'x y')
        >>> Point.__doc__                   # docstring for the new class
        'Point(x, y)'
        >>> p = Point(11, y=22)             # instantiate with positional args or keywords
        >>> p[0] + p[1]                     # indexable like a plain tuple
        33
        >>> x, y = p                        # unpack like a regular tuple
        >>> x, y
        (11, 22)
        >>> p.x + p.y                       # fields also accessable by name
        33
        >>> d = p._asdict()                 # convert to a dictionary
        >>> d['x']
        11
        >>> Point(**d)                      # convert from a dictionary
        Point(x=11, y=22)
        >>> p._replace(x=100)               # _replace() is like str.replace() but targets named fields
        Point(x=100, y=22)

        """

        # Parse and validate the field names.  Validation serves two purposes,
        # generating informative error messages and preventing template injection attacks.
        if isinstance(field_names, basestring):
            field_names = field_names.replace(',', ' ').split() # names separated by whitespace and/or commas
        field_names = tuple(map(str, field_names))
        for name in (typename,) + field_names:
            if not all(c.isalnum() or c=='_' for c in name):
                raise ValueError('Type names and field names can only contain alphanumeric characters and underscores: %r' % name)
            if _iskeyword(name):
                raise ValueError('Type names and field names cannot be a keyword: %r' % name)
            if name[0].isdigit():
                raise ValueError('Type names and field names cannot start with a number: %r' % name)
        seen_names = set()
        for name in field_names:
            if name.startswith('_'):
                raise ValueError('Field names cannot start with an underscore: %r' % name)
            if name in seen_names:
                raise ValueError('Encountered duplicate field name: %r' % name)
            seen_names.add(name)

        # Create and fill-in the class template
        numfields = len(field_names)
        argtxt = repr(field_names).replace("'", "")[1:-1]   # tuple repr without parens or quotes
        reprtxt = ', '.join('%s=%%r' % name for name in field_names)
        dicttxt = ', '.join('%r: t[%d]' % (name, pos) for pos, name in enumerate(field_names))
        template = '''class %(typename)s(tuple):
        '%(typename)s(%(argtxt)s)' \n
        __slots__ = () \n
        _fields = %(field_names)r \n
        def __new__(_cls, %(argtxt)s):
            return _tuple.__new__(_cls, (%(argtxt)s)) \n
        @classmethod
        def _make(cls, iterable, new=tuple.__new__, len=len):
            'Make a new %(typename)s object from a sequence or iterable'
            result = new(cls, iterable)
            if len(result) != %(numfields)d:
                raise TypeError('Expected %(numfields)d arguments, got %%d' %% len(result))
            return result \n
        def __repr__(self):
            return '%(typename)s(%(reprtxt)s)' %% self \n
        def _asdict(t):
            'Return a new dict which maps field names to their values'
            return {%(dicttxt)s} \n
        def _replace(_self, **kwds):
            'Return a new %(typename)s object replacing specified fields with new values'
            result = _self._make(map(kwds.pop, %(field_names)r, _self))
            if kwds:
                raise ValueError('Got unexpected field names: %%r' %% kwds.keys())
            return result \n
        def __getnewargs__(self):
            return tuple(self) \n\n''' % locals()
        for i, name in enumerate(field_names):
            template += '        %s = _property(_itemgetter(%d))\n' % (name, i)
        if verbose:
            print template

        # Execute the template string in a temporary namespace and
        # support tracing utilities by setting a value for frame.f_globals['__name__']
        namespace = dict(_itemgetter=_itemgetter, __name__='namedtuple_%s' % typename,
                         _property=property, _tuple=tuple)
        try:
            exec template in namespace
        except SyntaxError, e:
            raise SyntaxError(e.message + ':\n' + template)
        result = namespace[typename]

        # For pickling to work, the __module__ variable needs to be set to the frame
        # where the named tuple is created.  Bypass this step in enviroments where
        # sys._getframe is not defined (Jython for example).
        if hasattr(_sys, '_getframe'):
            result.__module__ = _sys._getframe(1).f_globals.get('__name__', '__main__')

        return result
# Représente une position sur le terrain du jeu.
position = namedtuple("position",
    'x ' # <- coordonnée X
    'y ' # <- coordonnée Y
)


# Contient les informations sur la taille du terrain du jeu.
taille_terrain = namedtuple("taille_terrain",
    'taille ' # <- taille actuelle du terrain
    'min_coord ' # <- coordonnée minimale en X ou en Y
    'max_coord ' # <- coordonnée maximale en X ou en Y
)


# Donne les caractéristiques d'un type d'unité.
caracs = namedtuple("caracs",
    'pa_init ' # <- nombre de points d'actions par tour
    'portee ' # <- portée maximale de l'unité
)


# Représente une unité sur le terrain.
unite = namedtuple("unite",
    'pos ' # <- la position de l'unité
    'ennemi ' # <- vrai si l'unité appartient à l'ennemi
    'type_unite_actuel ' # <- le type de l'unité, qui change si l'unité est déguisée
    'vrai_type_unite ' # <- le vrai type de l'unité (qui ne change pas lors du déguisement)
    'ko ' # <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité
    'pa ' # <- le nombre de PA restants à l'unité
    'attaques ' # <- le nombre d'attaques restants à l'unité
    'attaques_gratuites ' # <- le nombre d'attaques gratuites (voir la partie banzai)
    'id ' # <- l'identifiant unique de l'unité
)


# Représente l'ensemble des cartes que vous pouvez utiliser.
cartes = namedtuple("cartes",
    'potion ' # <- le nombre de cartes « Potion magique »
    'deguisement ' # <- le nombre de cartes « Déguisement »
    'banzai ' # <- le nombre de cartes « Banzaï »
    'pacifisme ' # <- le nombre de cartes « Pacifisme »
)


