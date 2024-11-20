using SFML.Graphics;
using SFML.System;
using System.Net.Http.Headers;

namespace Drakborgen
{
    internal class AnimationManager
    {
        private record Animation(Transformable Transformable, Vector2f Start, Vector2f End, float Time, float StartTime, Action DoneCallback)
        {
            internal float StartTime { get; set; } = StartTime;
        }

        private List<Animation> _animations = new List<Animation>();

        internal void Update(float elapsedTime, float timeDelta)
        {
            List<Action> doneCallbacks = new List<Action>();
            for (int i = (int)(_animations.Count) - 1; i >= 0; --i)
            {
                if (_animations[i].StartTime == 0.0f)
                {
                    _animations[i].StartTime = elapsedTime;
                }
                else
                {
                    float timeProgress = (elapsedTime - _animations[i].StartTime) / _animations[i].Time;
                    if (timeProgress >= 1.0f)
                    {
                        _animations[i].Transformable.Position = _animations[i].End;
                        doneCallbacks.Add(_animations[i].DoneCallback);
                        _animations.RemoveAt(i);
                        break;
                    }
                    else
                    {
                        float spaceProgress = (float)Math.Sqrt(1.0f - Math.Pow(1.0f - timeProgress, 2.0f));
                        Vector2f movement = (_animations[i].End - _animations[i].Start) * spaceProgress;
                        _animations[i].Transformable.Position = _animations[i].Start + movement;
                    }
                }
            }
            foreach (Action callback in doneCallbacks)
            {
                callback();
            }
        }

        internal void Add(Transformable transformable, Vector2f target, float time, Action doneCallback)
        {
            Animation newAnimation = new Animation(transformable, transformable.Position, target, time, 0.0f, doneCallback);

            int i = 0;
            for (; i < _animations.Count; ++i)
            {
                if (_animations[i].Transformable == transformable)
                {
                    break;
                }
            }
            if (i == _animations.Count)
            {
                _animations.Add(newAnimation);
            }
            else
            {
                if (_animations[i].Transformable != null && _animations[i].Transformable != transformable)
                {
                    throw new Exception();
                }
                _animations[i] = newAnimation;
            }
        }

        internal void Remove(Transformable transformable)
        {
            for (int i = 0; i < _animations.Count; ++i)
            {
                if (_animations[i].Transformable == transformable)
                {
                    _animations.RemoveAt(i);
                    break;
                }
            }
        }
    }
}