% [x, converged] = ITP_solve(fhandle, a, b)
%
% Solves a univariate function for approximate equality to zero using the
% ITP Method described in the paper
% I.F.D. Oliveira and R.H.C. Takahashi. 2020. An Enhancement of the Bisection
% Method Average Performance Preserving Minmax Optimality. ACM Trans. Math.
% Softw. 47, 1, Article 5 (December 2020), 24 pages.
% https://doi.org/10.1145/3423597
%
% ---INPUTS---
% fhandle is a function handle for the univariate function to be solved.
%         This function must take one input argument. Wrapping with an
%         anonymous function may be used:
%         x2 = constant;
%         fh = @(x1) func(x1, x2);
%         [x, converged] = ITP_solve(fh, a, b);
%
% a and b are arguments to fhandle such that a ~= b and either
%         fhandle(a) < 0 and fhandle(b) > 0 or
%         fhandle(a) > 0 and fhandle(b) < 0.
%
% ---OUTPUTS---
% x is the solver output. It is the argument to fhandle such that
%         fhandle(x0) approximately equals 0, abs(x-x0) <= tol.
%         This only holds if converged is true.
%         tol is defined as 2*max(eps(a), eps(b)).
%
% converged is true if the solver converged to a solution. converged is false if
%         the solver failed to converge.
%
function [x, converged] = ITP_solve(fhandle, a, b)
  %
  % Written in 2023 by Ben Tesch.
  % Originally distributed at https://github.com/slugrustle/numerical_routines
  %
  % To the extent possible under law, the author has dedicated all copyright
  % and related and neighboring rights to this software to the public domain
  % worldwide. This software is distributed without any warranty.
  % The text of the CC0 Public Domain Dedication should be reproduced at the
  % end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
  %

  % Input checks.
  assert(is_function_handle(fhandle), 'fhandle must be a function handle');
  assert(isa(a, 'double') && isreal(a) && isfinite(a) && isscalar(a), 'a must be a real, finite, scalar, double precision floating point value.');
  assert(isa(b, 'double') && isreal(b) && isfinite(b) && isscalar(b), 'b must be a real, finite, scalar, double precision floating point value.');

  % Initialization.
  tol = 2*max(eps(a), eps(b));
  nonzero = 2*eps(double(1));
  converged = true;

  % Ensure a < b.
  if a > b,
    c = a;
    a = b;
    b = c;
  end

  % Max iterations for ITP method. n0 is a tuning parameter.
  n0 = 1;
  n_max_bisec = ceil(log2((b-a)/(2*tol)));
  n_max_ITP = n_max_bisec + n0;

  % Other ITP method tuning parameters.
  k1 = 0.1;
  k2 = 2;

  % Initial ya & yb values and checks on the same.
  ya = fhandle(a);
  yb = fhandle(b);
  assert(isa(ya, 'double') && isreal(ya) && isfinite(ya) && isscalar(ya), 'fhandle must return a real, finite, scalar, double precision floating point result.');
  assert(isa(yb, 'double') && isreal(yb) && isfinite(yb) && isscalar(yb), 'fhandle must return a real, finite, scalar, double precision floating point result.');

  if ya == 0,
    x = a;
    fval = ya;
    return;
  end

  if yb == 0,
    x = b;
    fval = yb;
    return;
  end

  assert(a~=b, 'a and b must not be equal.');
  assert((ya < 0 && yb > 0) || (ya > 0 && yb < 0), 'fhandle(a) and fhandle(b) must have opposite signs.');

  % Main algorithm iteration loop.
  nIter = 0;
  while b-a > 2*tol,
    % x estimate by interpolation.
    x_interp = a + max(b-a, nonzero) * abs(ya) / max(abs(ya-yb), nonzero);

    % x estimate via bisection.
    x_bisec = min(max(0.5*(a+b), a), b);

    % x estimate by truncation.
    diff_bisec_interp = x_bisec - x_interp;
    sign_diff_bisec_interp = sign(diff_bisec_interp);
    abs_diff_bisec_interp = abs(diff_bisec_interp);

    delta = 0.2*(b-a);
    if b-a <= 2,
      delta = k1 * max(b-a, nonzero)^k2;
    end

    x_trunc = x_bisec;
    if delta <= abs_diff_bisec_interp,
      x_trunc = x_interp + sign_diff_bisec_interp * delta;
    end

    % Radius of minmax interval.
    r = max(0, tol*2^(n_max_ITP-nIter)-0.5*b+0.5*a-nonzero);

    % x estimate for this iteration.
    x_estimate = x_bisec - sign_diff_bisec_interp*r;
    if abs_diff_bisec_interp <= r,
      x_estimate = x_trunc;
    end

    % Update values based on new x estimate.
    nIter = nIter + 1;
    y_estimate = fhandle(x_estimate);
    if ya > 0 && yb < 0,
      if y_estimate > 0,
        a = x_estimate;
        ya = y_estimate;
      elseif y_estimate < 0,
        b = x_estimate;
        yb = y_estimate;
      else
        a = x_estimate;
        b = x_estimate;
        break;
      end
    elseif ya < 0 && yb > 0,
      if y_estimate > 0,
        b = x_estimate;
        yb = y_estimate;
      elseif y_estimate < 0,
        a = x_estimate;
        ya = y_estimate;
      else
        a = x_estimate;
        b = x_estimate;
        break;
      end
    else
      % Early termination due to interval sign error.
      % I *think* this shouldn't happen.
      converged = false;
      break;
    end

    if nIter-1 > n_max_ITP,
      % Early termination due to iteration limit from algorithm convergence
      % properties.
      converged = false;
      break;
    end
  end

  % Compute output value prior to returning.
  x = 0.5 * (a+b);
end
%
% Creative Commons Legal Code
%
% CC0 1.0 Universal
%
% CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
% LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
% ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
% INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
% REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
% PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
% THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
% HEREUNDER.
%
% Statement of Purpose
%
% The laws of most jurisdictions throughout the world automatically confer
% exclusive Copyright and Related Rights (defined below) upon the creator
% and subsequent owner(s) (each and all, an "owner") of an original work of
% authorship and/or a database (each, a "Work").
%
% Certain owners wish to permanently relinquish those rights to a Work for
% the purpose of contributing to a commons of creative, cultural and
% scientific works ("Commons") that the public can reliably and without fear
% of later claims of infringement build upon, modify, incorporate in other
% works, reuse and redistribute as freely as possible in any form whatsoever
% and for any purposes, including without limitation commercial purposes.
% These owners may contribute to the Commons to promote the ideal of a free
% culture and the further production of creative, cultural and scientific
% works, or to gain reputation or greater distribution for their Work in
% part through the use and efforts of others.
%
% For these and/or other purposes and motivations, and without any
% expectation of additional consideration or compensation, the person
% associating CC0 with a Work (the "Affirmer"), to the extent that he or she
% is an owner of Copyright and Related Rights in the Work, voluntarily
% elects to apply CC0 to the Work and publicly distribute the Work under its
% terms, with knowledge of his or her Copyright and Related Rights in the
% Work and the meaning and intended legal effect of CC0 on those rights.
%
% 1. Copyright and Related Rights. A Work made available under CC0 may be
% protected by copyright and related or neighboring rights ("Copyright and
% Related Rights"). Copyright and Related Rights include, but are not
% limited to, the following:
%
% i. the right to reproduce, adapt, distribute, perform, display,
% communicate, and translate a Work;
% ii. moral rights retained by the original author(s) and/or performer(s);
% iii. publicity and privacy rights pertaining to a person's image or
% likeness depicted in a Work;
% iv. rights protecting against unfair competition in regards to a Work,
% subject to the limitations in paragraph 4(a), below;
% v. rights protecting the extraction, dissemination, use and reuse of data
% in a Work;
% vi. database rights (such as those arising under Directive 96/9/EC of the
% European Parliament and of the Council of 11 March 1996 on the legal
% protection of databases, and under any national implementation
% thereof, including any amended or successor version of such
% directive); and
% vii. other similar, equivalent or corresponding rights throughout the
% world based on applicable law or treaty, and any national
% implementations thereof.
%
% 2. Waiver. To the greatest extent permitted by, but not in contravention
% of, applicable law, Affirmer hereby overtly, fully, permanently,
% irrevocably and unconditionally waives, abandons, and surrenders all of
% Affirmer's Copyright and Related Rights and associated claims and causes
% of action, whether now known or unknown (including existing as well as
% future claims and causes of action), in the Work (i) in all territories
% worldwide, (ii) for the maximum duration provided by applicable law or
% treaty (including future time extensions), (iii) in any current or future
% medium and for any number of copies, and (iv) for any purpose whatsoever,
% including without limitation commercial, advertising or promotional
% purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
% member of the public at large and to the detriment of Affirmer's heirs and
% successors, fully intending that such Waiver shall not be subject to
% revocation, rescission, cancellation, termination, or any other legal or
% equitable action to disrupt the quiet enjoyment of the Work by the public
% as contemplated by Affirmer's express Statement of Purpose.
%
% 3. Public License Fallback. Should any part of the Waiver for any reason
% be judged legally invalid or ineffective under applicable law, then the
% Waiver shall be preserved to the maximum extent permitted taking into
% account Affirmer's express Statement of Purpose. In addition, to the
% extent the Waiver is so judged Affirmer hereby grants to each affected
% person a royalty-free, non transferable, non sublicensable, non exclusive,
% irrevocable and unconditional license to exercise Affirmer's Copyright and
% Related Rights in the Work (i) in all territories worldwide, (ii) for the
% maximum duration provided by applicable law or treaty (including future
% time extensions), (iii) in any current or future medium and for any number
% of copies, and (iv) for any purpose whatsoever, including without
% limitation commercial, advertising or promotional purposes (the
% "License"). The License shall be deemed effective as of the date CC0 was
% applied by Affirmer to the Work. Should any part of the License for any
% reason be judged legally invalid or ineffective under applicable law, such
% partial invalidity or ineffectiveness shall not invalidate the remainder
% of the License, and in such case Affirmer hereby affirms that he or she
% will not (i) exercise any of his or her remaining Copyright and Related
% Rights in the Work or (ii) assert any associated claims and causes of
% action with respect to the Work, in either case contrary to Affirmer's
% express Statement of Purpose.
%
% 4. Limitations and Disclaimers.
%
% a. No trademark or patent rights held by Affirmer are waived, abandoned,
% surrendered, licensed or otherwise affected by this document.
% b. Affirmer offers the Work as-is and makes no representations or
% warranties of any kind concerning the Work, express, implied,
% statutory or otherwise, including without limitation warranties of
% title, merchantability, fitness for a particular purpose, non
% infringement, or the absence of latent or other defects, accuracy, or
% the present or absence of errors, whether or not discoverable, all to
% the greatest extent permissible under applicable law.
% c. Affirmer disclaims responsibility for clearing rights of other persons
% that may apply to the Work or any use thereof, including without
% limitation any person's Copyright and Related Rights in the Work.
% Further, Affirmer disclaims responsibility for obtaining any necessary
% consents, permissions or other rights required for any use of the
% Work.
% d. Affirmer understands and acknowledges that Creative Commons is not a
% party to this document and has no duty or obligation with respect to
% this CC0 or use of the Work.
%
