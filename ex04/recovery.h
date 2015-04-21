/* Authors:
 * - Huber Lukas
 * - Alexander Hirsch
 * - Patrick Ober
 * - Michael Tscholl
 * - Franz Josef Haider
 */

#ifndef RECOVERY_H
#define RECOVERY_H

/* key value pair: rule -> tokens */
struct rule_token_pair {
    /* key */
    void (*rule)(void);

    /* value */
    unsigned int num;
    enum token_type *types;
};

/* returns pair with given key or NULL */
const struct rule_token_pair *recovery_lookup_first(void (*key)(void));

/* returns pair with given key or NULL */
const struct rule_token_pair *recovery_lookup_follow(void (*key)(void));

#endif
